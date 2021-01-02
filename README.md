# mahjong-cpp

## About (概要)

日本のリーチ麻雀のプログラム置き場

Miscellaneous programs about Japanese Mahjong

## Features (機能)

🚧This program is under development. Currently the following features have been implemented.🚧

* [x] 向聴数計算 (Syanten Number Calculation)
* [x] 点数計算 (Score Calculation)
* [x] 有効牌列挙 (Required Tile Selection)
* [x] 不要牌列挙 (Unnecessary Tile Selection)
* [x] 期待値計算 (Expected Score Calculation)
  * [x] 向聴戻し考慮
  * [x] 一発、海底撈月考慮
  * [ ] 裏ドラ考慮
  * [ ] 向聴数が変化しない手変わり考慮
  * [ ] 槓されている場合を考慮

## Requirements (依存ライブラリ)

* [Boost C++ Libraries](https://www.boost.org/) >= 1.61
* [CMake](https://cmake.org/) >= 3.1.1

## How to build (ビルド方法)

```bash
git clone https://github.com/nekobean/mahjong-cpp.git
cd mahjong-cpp
mkdir build
cd build
cmake ..
```

## Usage (使い方)

* [向聴数計算 (Syanten Number Calculation)](src/samples/sample_calculate_syanten.cpp)
* [点数計算 (Score Calculation)](src/samples/sample_calculate_score.cpp)
* [有効牌選択 (Required Tile Selection)](src/samples/sample_required_tile_selector.cpp)
* [不要牌選択 (Unnecessary Tile Selection)](src/samples/sample_unnecessary_tile_selector.cpp)
* [期待値計算 (Expected Value Calculation)](src/samples/sample_calculate_expexted_value.cpp)

### 点数計算の例

```cpp
#include "mahjong/mahjong.hpp"

using namespace mahjong;

int main(int, char **)
{
    ScoreCalculator score;

    // 場やルールの設定
    score.set_bakaze(Tile::Ton);  // 場風牌
    score.set_zikaze(Tile::Ton);  // 自風牌
    score.set_num_tumibo(0);  // 積み棒の数
    score.set_num_kyotakubo(0);  // 供託棒の数
    score.set_dora_tiles({Tile::Pe});  // ドラの一覧 (表示牌ではない)
    score.set_uradora_tiles({Tile::Pinzu9});  // 裏ドラの一覧 (表示牌ではない)

    // 手牌、和了牌、フラグの設定
    // 手牌
    MeldedBlock block(MeldType::Kakan, {Tile::Ton, Tile::Ton, Tile::Ton, Tile::Ton});
    Hand hand({Tile::Manzu1, Tile::Manzu2, Tile::Manzu3, Tile::Pinzu3, Tile::Pinzu4,
                Tile::Pinzu5, Tile::Sozu1, Tile::Sozu2, Tile::Sozu3, Tile::Sozu4, Tile::Sozu4},
                {block});
    int win_tile = Tile::Manzu1;  // 和了牌
    int flag = HandFlag::Tumo | HandFlag::Rinsyankaiho;  // フラグ

    // 点数計算
    Result ret = score.calc(hand, win_tile, flag);
    std::cout << ret.to_string() << std::endl;
}
```

```output
[結果]
手牌: 123m 345p 12344s [東東東東, 加槓], 和了牌: 一萬, 自摸
面子構成:
  [東東東東, 明槓子]
  [一萬二萬三萬, 暗順子]
  [三筒四筒五筒, 暗順子]
  [一索二索三索, 暗順子]
  [四索四索, 暗対子]
待ち: 両面待ち
役:
 嶺上開花 1翻
 自風 (東) 1翻
 場風 (東) 1翻
40符3翻
```

### 期待値計算の例

```
手牌: 2345m 113479p 6778s, 向聴数: 1, 巡目: 1
[打 九筒 (向聴戻し)] 有効牌: 19種64枚, 聴牌確率: 87.95%, 和了確率: 39.02%, 期待値: 1843.78 有効牌 一萬 一筒 二萬 二筒 三萬 四萬 五萬 五筒 五索 六萬 六筒 六索 七萬 七筒 
七索 八筒 八索 九筒 九索
[打 七筒 (向聴戻し)] 有効牌: 18種60枚, 聴牌確率: 87.50%, 和了確率: 37.27%, 期待値: 1752.13 有効牌 一萬 一筒 二萬 二筒 三萬 四萬 五萬 五筒 五索 六萬 六索 七萬 七筒 七索 
八筒 八索 九筒 九索
[打 二萬] 有効牌: 3種12枚, 聴牌確率: 85.23%, 和了確率: 32.41%, 期待値: 1563.97 有効牌 二筒 五筒 八筒
[打 五萬] 有効牌: 3種12枚, 聴牌確率: 85.23%, 和了確率: 32.41%, 期待値: 1563.97 有効牌 二筒 五筒 八筒
[打 七索] 有効牌: 3種12枚, 聴牌確率: 85.23%, 和了確率: 33.65%, 期待値: 1541.07 有効牌 二筒 五筒 八筒
[打 四筒 (向聴戻し)] 有効牌: 18種60枚, 聴牌確率: 79.70%, 和了確率: 25.55%, 期待値: 1110.55 有効牌 一萬 一筒 二萬 二筒 三萬 三筒 四萬 四筒 五萬 五筒 五索 六萬 六索 七萬 
七索 八筒 八索 九索
[打 三筒 (向聴戻し)] 有効牌: 19種64枚, 聴牌確率: 76.47%, 和了確率: 24.38%, 期待値: 1064.24 有効牌 一萬 一筒 二萬 二筒 三萬 三筒 四萬 四筒 五萬 五筒 五索 六萬 六筒 六索 
七萬 七索 八筒 八索 九索
[打 八索 (向聴戻し)] 有効牌: 7種23枚, 聴牌確率: 76.19%, 和了確率: 23.76%, 期待値: 1045.61 有効牌 一筒 二筒 五筒 五索 七索 八筒 八索
[打 六索 (向聴戻し)] 有効牌: 7種23枚, 聴牌確率: 76.19%, 和了確率: 23.76%, 期待値: 1045.61 有効牌 一筒 二筒 五筒 六索 七索 八筒 九索
[打 三萬 (向聴戻し)] 有効牌: 5種19枚, 聴牌確率: 67.61%, 和了確率: 20.34%, 期待値: 932.00 有効牌 二筒 三萬 五筒 六萬 八筒
[打 四萬 (向聴戻し)] 有効牌: 5種19枚, 聴牌確率: 67.61%, 和了確率: 20.34%, 期待値: 932.00 有効牌 一萬 二筒 四萬 五筒 八筒
[打 一筒 (向聴戻し)] 有効牌: 8種25枚, 聴牌確率: 73.76%, 和了確率: 21.89%, 期待値: 911.94 有効牌 一筒 二萬 二筒 四筒 五萬 五筒 七索 八筒
計算時間: 5194us
```

## Benchmark (ベンチマーク)

* Core™ i9-9900K 3.6 GHz

### 向聴数計算

|      | N=40000 | 1手あたりの平均計算時間  |
|------|--------------------|-------------|
| 一般手  | 576 us           | 14.4ns |
| 七対子手 | 240 us          | 6.0ns |
| 国士手  | 147 us           | 3.7ns |

### 点数計算

|      | N=410831 | 1手あたりの平均計算時間  |
|------|--------------------|-------------|
| 一般手  | 128ms              | 311ns |

### 不要牌選択

|      | N=100000 | 1手あたりの平均計算時間  |
|------|--------------------|-------------|
| 一般手  | 33 ms           | 330ns |
| 七対子手 | 21 ms          | 210ns |
| 国士手  | 12 ms           | 120ns |

### 有効牌選択

|      | N=100000 | 1手あたりの平均計算時間  |
|------|--------------------|-------------|
| 一般手  | 62 ms           | 620ns |
| 七対子手 | 35 ms          | 350ns |
| 国士手  | 13 ms           | 130ns |
