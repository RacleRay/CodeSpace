import asyncio
import aiohttp
from bs4 import BeautifulSoup
import os
from urllib.parse import urljoin
import time
from typing import List
import logging
import random
from aiohttp import ClientTimeout
from tenacity import retry, stop_after_attempt, wait_exponential

logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)


class WebCrawler:
    USER_AGENTS = [
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko/20100101 Firefox/89.0",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.1.1 Safari/605.1.15",
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36 Edg/91.0.864.59",
    ]

    def __init__(
        self,
        base_url: str,
        output_dir: str,
        delay: float = 15.0,
    ):
        self.base_url = base_url
        self.output_dir = output_dir
        self.delay = delay
        self.session = None
        self.last_request_time = 0
        self.timeout = ClientTimeout(total=30)

    def get_random_headers(self) -> dict:
        return {
            "User-Agent": random.choice(self.USER_AGENTS),
            "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
            "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8",
            "Accept-Encoding": "gzip, deflate, br",
            "Connection": "keep-alive",
            "DNT": "1",
            "Upgrade-Insecure-Requests": "1",
        }

    async def init_session(self):
        if not self.session:
            self.session = aiohttp.ClientSession(timeout=self.timeout)

    async def close_session(self):
        if self.session:
            await self.session.close()
            self.session = None

    @retry(
        stop=stop_after_attempt(10),
        wait=wait_exponential(multiplier=1, min=10, max=60),
    )
    async def fetch_page(self, url: str) -> str:
        current_time = time.time()
        time_since_last = current_time - self.last_request_time
        if time_since_last < self.delay:
            await asyncio.sleep(self.delay)

        try:
            headers = self.get_random_headers()
            async with self.session.get(url, headers=headers, ssl=False) as response:
                self.last_request_time = time.time()

                if response.status == 429:  # Too Many Requests
                    retry_after = int(response.headers.get("Retry-After", 60))
                    logging.info(f"触发限流，等待 {retry_after} 秒后重试...")
                    await asyncio.sleep(retry_after)
                    raise Exception("Rate limited, retrying...")

                response.raise_for_status()
                return await response.text()

        except Exception as e:
            logging.error(f"获取页面失败 {url}: {str(e)}")
            raise  # trigger retry

    async def get_section_title(self, html: str) -> str:
        soup = BeautifulSoup(html, "html.parser")
        title_element = soup.select_one(
            "body > div > div.off-canvas-content > div.columns > div > div.book-content > div.book-post > h1"
        )
        return title_element.text.strip() if title_element else "未命名专栏"

    async def get_section_links_with_titles(self, html: str) -> List[dict]:
        soup = BeautifulSoup(html, "html.parser")
        links = soup.select(
            "body > div > div.off-canvas-content > div.columns > div > div.book-content > div.book-post > div:nth-child(4) > ul > li > a"
        )
        return [
            {
                "url": urljoin(self.base_url, link["href"]),
                "title": link.text.strip(),
            }
            for link in links
        ]

    async def download_image(self, img_url: str, save_path: str):
        try:
            os.makedirs(os.path.dirname(save_path), exist_ok=True)

            headers = self.get_random_headers()
            async with self.session.get(
                img_url, headers=headers, ssl=False
            ) as response:
                response.raise_for_status()
                content = await response.read()

                with open(save_path, "wb") as f:
                    f.write(content)
                logging.info(f"保存图片: {save_path}")

        except Exception as e:
            logging.error(f"下载图片失败 {img_url}: {str(e)}")

    async def process_html_content(
        self, html: str, save_dir: str, page_url: str
    ) -> str:
        soup = BeautifulSoup(html, "html.parser")

        page_base_url = page_url.rsplit("/", 1)[0] + "/"

        skip_images = {"/static/favicon.png"}

        for img in soup.find_all("img"):
            src = img.get("src")
            if not src or src in skip_images:
                continue

            if not src.startswith(("http://", "https://")):
                full_url = urljoin(page_base_url, src)
            else:
                full_url = src

            img_filename = os.path.basename(src)
            local_path = os.path.join(save_dir, "assets", img_filename)

            await self.download_image(full_url, local_path)

            img["src"] = os.path.join("assets", img_filename)

        return str(soup)

    def save_content(self, url: str, content: str, save_dir: str):
        filename = os.path.basename(url.split(self.base_url)[-1].strip("/"))
        if not filename:
            filename = "index"
        file_path = os.path.join(save_dir, f"{filename}.html")

        with open(file_path, "w", encoding="utf-8") as f:
            f.write(content)
        logging.info(f"保存文件: {file_path}")

    async def process_section(self, section_info: dict):
        section_url = section_info["url"]
        section_title = section_info["title"]

        safe_title = "".join(
            c for c in section_title if c.isalnum() or c in (" ", "-", "_")
        ).strip()
        save_dir = os.path.join(self.output_dir, safe_title)

        if os.path.exists(save_dir):
            logging.info(f"跳过已存在的目录: {save_dir}")
            return

        try:
            html = await self.fetch_page(section_url)
            if not html:
                return

            doc_links = await self.get_document_links(section_url)
            logging.info(f"开始处理专栏 '{section_title}', 共 {len(doc_links)} 个文档")

            for i, doc_url in enumerate(doc_links, 1):
                try:
                    logging.info(
                        f"正在处理 '{section_title}' 的第 {i}/{len(doc_links)} 个文档"
                    )
                    await self.extract_and_save_content(doc_url, save_dir)
                except Exception as e:
                    logging.error(f"处理文档失败 {doc_url}: {str(e)}")
                    continue

        except Exception as e:
            logging.error(f"处理专栏失败 {section_url}: {str(e)}")

    async def extract_and_save_content(self, url: str, save_dir: str):
        html = await self.fetch_page(url)
        if not html:
            return

        processed_html = await self.process_html_content(html, save_dir, url)

        self.save_content(url, processed_html, save_dir)

    async def get_document_links(self, section_url: str) -> List[str]:
        html = await self.fetch_page(section_url)
        if not html:
            return []

        soup = BeautifulSoup(html, "html.parser")
        links = soup.select(
            "body > div > div.off-canvas-content > div.columns > div > div.book-content > div.book-post > div:nth-child(4) > ul > li > a"
        )
        return [urljoin(self.base_url, link["href"]) for link in links]

    async def crawl(self):
        await self.init_session()
        try:
            home_html = await self.fetch_page(self.base_url)
            sections = await self.get_section_links_with_titles(home_html)

            logging.info(f"共发现 {len(sections)} 个专栏")

            for i, section in enumerate(sections, 1):
                logging.info(
                    f"开始处理第 {i}/{len(sections)} 个专栏: {section['title']}"
                )
                await self.process_section(section)

        finally:
            await self.close_session()


async def main():
    base_url = "https://learn.lianglianglee.com/"
    output_dir = "output"

    # single thread and serial, because of rate limit
    crawler = WebCrawler(
        base_url=base_url,
        output_dir=output_dir,
        delay=5.0,
    )

    await crawler.crawl()


if __name__ == "__main__":
    asyncio.run(main())
