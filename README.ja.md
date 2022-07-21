# MM138
https://www.topcoder.com/challenges/81d26476-fb3c-4e4e-9ffe-7ee17c0438ab?tab=submissions

こっちは主に自分用に環境構築などのメモを残す

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
mkdir build
cd build
cmake ..
make
src/run < ../data/1.in > ../data/1.mout
```

```
java -jar tester.jar -exec "../build/src/run" -seed 1,100 -saveAll ../data -novis -ss 0718.txt -th 8
```

### visualizer
- `$id.in`: 入力
- `$id.mout`: 出力。outputを複数回含めても良い
```
export id=2 && python3 visualizer.py -i ../data/$id.in -o ../data/$id.mout
```