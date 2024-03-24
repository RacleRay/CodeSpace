from db.user_db import UserDb


class UserService:
    __user_db=UserDb()
    def login(self, username, password):
        result = self.__user_db.login(username, password)
        return result

    def search_user_role(self, username):
        '查询用户角色'
        role = self.__user_db.search_user_role(username)
        return role

    def insert(self, username, password, email, role_id):
        '添加记录'
        self.__user_db.insert(username, password, email, role_id)

    def search_count_page(self):
        '查询用户总页数'
        count_page = self.__user_db.search_count_page()
        return count_page

    def search_list(self, page):
        '查询用户分页记录'
        result = self.__user_db.search_list(page)
        return result

    def update(self, id, username, password, email, role_id):
        '修改用户信息'
        self.__user_db.update(id, username, password, email, role_id)

    def delete_by_id(self, id):
        '删除用户'
        self.__user_db.delete_by_id(id)

    def search_userid(self, username):
        uid = self.__user_db.search_userid(username)
        return uid