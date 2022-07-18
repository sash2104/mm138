## GUIをdocker containerから開くまで
mac側
https://qiita.com/uedashuhei/items/3f6f8612b5c4a2b00b1a に近いことをやる
```
brew install xquartz
open -a XQuartz

# セキュリティ設定変更 (ネットワーククライアントからの接続をon)

xhost + localhost
```

docker container側
```
sudo apt-get update
sudo apt-get install -y python3-tk python3-pip libx11-dev
pip install Pillow
export DISPLAY=host.docker.internal:0
```

コード動かして下記のようなエラー出なくなればOK
```
_tkinter.TclError: couldn't connect to display "host.docker.internal:0"
```

## 実行
```
java -jar tester.jar -exec "../build/src/run" -seed 1,100 -saveAll ../data -novis -ss 0718.txt -th 8
```