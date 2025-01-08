# TL;DR
C言語によるechoサーバーの実装。クライアントで送信したメッセージがそのままレスポンスとして返ってくる。
# 実装状況
- Done
    - エラーハンドリング
    - メッセージの送受信
    - DNS解決
- ToDo
    - IPv6対応
    - 大量のリクエスト処理
    - Graceful Shutdown

# Usage
## echoサーバーの起動
```
gcc -o server server.c
./server
```
## client側のリクエスト送信方法
clientサーバーの起動
```
gcc -o client client.c
./client ${message} ${domain}
```
