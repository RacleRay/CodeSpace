import re
import sys
import hashlib
import logging
import numbers
import collections
from itertools import groupby


def _hashfunc(x): # 使用的hash函数
    return int(hashlib.md5(x).hexdigest(), 32)


class Simhash(object):
    def __init__(self,
                 value,
                 f=64,
                 reg=r'[\w\u4e00-\u9fcc]+',
                 hashfunc=None,
                 log=None):
        """
        `f` is the dimensions of fingerprints

        `reg` is meaningful only when `value` is str and describes
        what is considered to be a letter inside parsed string. Regexp
        object can also be specified (some attempt to handle any letters
        is to specify reg=re.compile(r'\w', re.UNICODE))

        `hashfunc` accepts a utf-8 encoded string and returns a unsigned
        integer in at least `f` bits.
        """

        self.f = f
        self.reg = reg
        self.value = None

        if hashfunc is None:
            self.hashfunc = _hashfunc
        else:
            self.hashfunc = hashfunc

        if log is None:
            self.log = logging.getLogger("simhash")
        else:
            self.log = log

        if isinstance(value, Simhash):
            self.value = value.value
        elif isinstance(value, str):
            #  print("build by text")
            self.build_by_text(str(value))
        elif isinstance(value, collections.Iterable):
            self.build_by_features(value)
        elif isinstance(value, numbers.Integral):
            self.value = value
        else:
            raise Exception('Bad parameter with type {}'.format(type(value)))

    def __eq__(self, other):
        """
        Compare two simhashes by their value.

        :param Simhash other: The Simhash object to compare to
        """
        return self.value == other.value

    def _slide(self, content, width=4):
        return [
            content[i:i + width]
            for i in range(max(len(content) - width + 1, 1))
        ]

    def _tokenize(self, content):
        content = content.lower()
        content = ''.join(re.findall(self.reg, content))
        ans = self._slide(content)
        return ans

    def build_by_text(self, content):
        features = self._tokenize(content)
        features = {k: sum(1 for _ in g) for k, g in groupby(sorted(features))}
        return self.build_by_features(features)

    def build_by_features(self, features):
        """
        核心方法
        `features` might be a list of unweighted tokens (a weight of 1
                   will be assumed), a list of (token, weight) tuples or
                   a token -> weight dict.
        """
        v = [0] * self.f  # 初始化 [0,0,0,...]
        masks = [1 << i for i in range(self.f)]  # 二进制下[1000, 0100, 0010, ...]

        if isinstance(features, dict):
            features = features.items()

        for f in features:
            if isinstance(f, str):
                h = self.hashfunc(f.encode('utf-8'))  # hash成32位
                w = 1
            else:
                assert isinstance(f, collections.Iterable)
                h = self.hashfunc(f[0].encode('utf-8'))
                w = f[1]
            for i in range(self.f):
                # mask位置为1，则vi加上w，否则减去w
                v[i] += w if h & masks[i] else -w

        ans = 0
        for i in range(self.f):
            if v[i] > 0:  # 如果大于0，就把那一位变成1
                ans |= masks[i]
        self.value = ans

    def distance(self, another):
        """计算两个vector有多少个位置不一样"""
        assert self.f == another.f
        x = (self.value ^ another.value) & ((1 << self.f) - 1)  # (1 << self.f) - 1: self.f个位的1
        ans = 0
        while x: # bin(x)不全为0，即x非0
            ans += 1
            x &= x - 1   # bin计算，每算一次，低位的第一个1变为0
        return ans


class SimhashIndex(object):
    def __init__(self, objs, f=64, k=2, log=None):
        """
        使用Simhash进行相似字符串检索
        `objs` is a list of (obj_id, simhash)
        obj_id is a string, simhash is an instance of Simhash
        `f` is the same with the one for Simhash
        `k` is the tolerance
        """
        self.k = k
        self.f = f
        count = len(objs)

        if log is None:
            self.log = logging.getLogger("simhash")
        else:
            self.log = log

        self.log.info('Initializing %s data.', count)

        self.bucket = collections.defaultdict(set)

        for i, q in enumerate(objs):
            if i % 10000 == 0 or i == count - 1:
                self.log.info('%s/%s', i + 1, count)
            self.add(*q)

    def get_near_dups(self, simhash):
        """
        `simhash` is an instance of Simhash
        return a list of obj_id, which is in type of str
        """
        assert simhash.f == self.f

        ans = set()

        for key in self.get_keys(simhash):
            dups = self.bucket[key]
            self.log.debug('key:%s', key)
            if len(dups) > 200:
                self.log.warning('Big bucket found. key:%s, len:%s', key,
                                 len(dups))

            for dup in dups:
                sim2, obj_id = dup.split(',', 1)
                sim2 = Simhash(int(sim2, 16), self.f)

                d = simhash.distance(sim2)
                if d <= self.k:
                    ans.add(obj_id)
        return list(ans)

    def add(self, obj_id, simhash):
        """
        `obj_id` is a string
        `simhash` is an instance of Simhash
        """
        assert simhash.f == self.f

        for key in self.get_keys(simhash):
            v = '%x,%s' % (simhash.value, obj_id)
            self.bucket[key].add(v)

    def delete(self, obj_id, simhash):
        """
        `obj_id` is a string
        `simhash` is an instance of Simhash
        """
        assert simhash.f == self.f

        for key in self.get_keys(simhash):
            v = '%x,%s' % (simhash.value, obj_id)
            if v in self.bucket[key]:
                self.bucket[key].remove(v)

    @property
    def offsets(self):
        """
        You may optimize this method according to <http://www.wwwconference.org/www2007/papers/paper215.pdf>
        """
        return [self.f // (self.k + 1) * i for i in range(self.k + 1)]

    def get_keys(self, simhash):
        for i, offset in enumerate(self.offsets):
            if i == (len(self.offsets) - 1):
                m = 2**(self.f - offset) - 1
            else:
                m = 2**(self.offsets[i + 1] - offset) - 1
            c = simhash.value >> offset & m
            yield '%x:%x' % (c, i)

    def bucket_size(self):
        return len(self.bucket)


if __name__ == '__main__':
    data = {
    1: u'How are you? I am fine. blar blar blar blar blar Thanks.',
    2: u'How are you i am fine. blar blar blar blar blar Thanks.',
    3: u'This is a simhash test',
    }

    # 序号和hash值保存
    objs = [(str(k), Simhash(v)) for k, v in data.items()]

    # 建立SimhashIndex对象，`k` is the tolerance
    index = SimhashIndex(objs, k=10)
    print("相似文本Bucket数量：", index.bucket_size())

    # 输如需要查找的文本的hash值，get_near_dups获取相似文本
    s1 = Simhash(u'How are you i am fine. blar blar blar blar blar thanks')
    print("相似文本id：", index.get_near_dups(s1))

    # 加入data文本
    index.add('4', s1)
    print("相似文本id：", index.get_near_dups(s1))

    s2 = Simhash(u'How are you i am fine. blar blar blar thanks')
    index.add('5', s2)
    print("相似文本id：", index.get_near_dups(s1))