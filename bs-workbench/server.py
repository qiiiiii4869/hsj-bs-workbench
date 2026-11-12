"""Launcher retained for the existing Windows start/stop scripts."""
import argparse
import uvicorn
from api import app


def main():
    parser = argparse.ArgumentParser(description='HSJ 本机浏览器分析服务（FastAPI）')
    parser.add_argument('--port', type=int, default=8765)
    args = parser.parse_args()
    # One process owns the session token, chart snapshots and export jobs.
    server=uvicorn.Server(uvicorn.Config(app,host='127.0.0.1',port=args.port,workers=1,proxy_headers=False))
    app.state.shutdown=lambda:setattr(server,'should_exit',True)
    server.run()


if __name__ == '__main__':
    main()
