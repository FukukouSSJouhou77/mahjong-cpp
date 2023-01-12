#include "../mahjong/mahjong.hpp"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <pybind11/pybind11.h>
using namespace mahjong;
int sample_mod1() {

    // 聴牌の手牌
    Hand hand1({Tile::Manzu2, Tile::Manzu2, Tile::Manzu2, Tile::Manzu5, Tile::Manzu6, Tile::Manzu7,
                Tile::Pinzu3, Tile::Pinzu4, Tile::Pinzu5, Tile::Sozu3, Tile::Sozu3, Tile::Sozu6,
                Tile::Sozu6, Tile::Sozu7});
    // 1向聴の手牌
    Hand hand2({Tile::Manzu2, Tile::Manzu2, Tile::Manzu2, Tile::Manzu5, Tile::Manzu6, Tile::Manzu7,
                Tile::Pinzu3, Tile::Pinzu4, Tile::Sozu3, Tile::Sozu3, Tile::Sozu6, Tile::Sozu6,
                Tile::Sozu7, Tile::Pe});
    // 3向聴の手牌
    Hand hand3({Tile::Manzu1, Tile::Manzu1, Tile::Manzu2, Tile::Manzu4, Tile::Manzu5, Tile::Manzu7,
                Tile::Pinzu9, Tile::Sozu3, Tile::Sozu7, Tile::Sozu9, Tile::Ton, Tile::Pe, Tile::Pe,
                Tile::Hatu});
    // 2向聴の手牌
    Hand hand4({Tile::Manzu1, Tile::Manzu2, Tile::Manzu3, Tile::Manzu3, Tile::Manzu4, Tile::Manzu9,
                Tile::Pinzu3, Tile::Pinzu6, Tile::Pinzu8, Tile::Pinzu8, Tile::Sozu1, Tile::Sozu2,
                Tile::Sozu4, Tile::Sozu5});
    // 1向聴の手牌
    Hand hand5({Tile::Manzu1, Tile::Manzu2, Tile::Manzu4, Tile::Manzu5, Tile::Manzu5, Tile::Pinzu3,
                Tile::Pinzu4, Tile::Pinzu5, Tile::Pinzu6, Tile::Pinzu7, Tile::Sozu6, Tile::Sozu7,
                Tile::Sozu7, Tile::Sozu7});

    int bakaze = Tile::Ton;                 // 場風
    int zikaze = Tile::Ton;                 // 自風
    int turn = 3;                           // 巡目
    int syanten_type = SyantenType::Normal; // 向聴数の種類
    // 考慮する項目
    int flag = ExpectedValueCalculator::CalcSyantenDown   // 向聴戻し考慮
               | ExpectedValueCalculator::CalcTegawari    // 手変わり考慮
               | ExpectedValueCalculator::CalcDoubleReach // ダブル立直考慮
               | ExpectedValueCalculator::CalcIppatu      // 一発考慮
               | ExpectedValueCalculator::CalcHaiteitumo  // 海底撈月考慮
               | ExpectedValueCalculator::CalcUradora     // 裏ドラ考慮
               | ExpectedValueCalculator::CalcAkaTileTumo // 赤牌自摸考慮
        //| ExpectedValueCalculator::MaximaizeWinProb // 和了確率を最大化
        ;
    std::vector<int> dora_indicators = {Tile::Ton}; // ドラ表示牌
    Hand hand = hand5;                              // 手牌

    ExpectedValueCalculator exp_value_calculator;
    ScoreCalculator score_calculator;

    // 点数計算の設定
    score_calculator.set_bakaze(bakaze);
    score_calculator.set_zikaze(zikaze);
    score_calculator.set_dora_indicators(dora_indicators);

    // 向聴数を計算する。
    auto [_, syanten] = SyantenCalculator::calc(hand, syanten_type);

    // 期待値を計算する。
    auto begin = std::chrono::steady_clock::now();
    auto [success, candidates] =
        exp_value_calculator.calc(hand, score_calculator, dora_indicators, syanten_type, flag);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    if (!success) {
        std::cout << "エラー" << std::endl;
        return 1;
    }

    bool exp_value_calculated = !candidates[0].tenpai_probs.empty();

    // 期待値が高い順にソートする。
    if (exp_value_calculated)
        std::sort(candidates.begin(), candidates.end(),
                  [turn](const Candidate &a, const Candidate &b) {
                      return a.exp_values[turn - 1] > b.exp_values[turn - 1];
                  });

    // 結果を出力する。
    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << fmt::format("手牌: {}, 向聴数: {}, 巡目: {}", hand.to_string(), syanten, turn)
              << std::endl;
    std::cout << fmt::format("計算時間: {}us", elapsed_ms) << std::endl;

    for (const auto &candidate : candidates) {
        int sum_required_tiles =
            std::accumulate(candidate.required_tiles.begin(), candidate.required_tiles.end(), 0,
                            [](auto &a, auto &b) { return a + std::get<1>(b); });

        std::cout << fmt::format("[打 {}] 有効牌: {:>2d}種{:>2d}枚", Tile::Name.at(candidate.tile),
                                 candidate.required_tiles.size(), sum_required_tiles);

        if (exp_value_calculated)
            std::cout << fmt::format(", 聴牌確率: {:>5.2f}%, 和了確率: {:>5.2f}%, 期待値: {:>7.2f}",
                                     candidate.tenpai_probs[turn - 1] * 100,
                                     candidate.win_probs[turn - 1] * 100,
                                     candidate.exp_values[turn - 1]);
        if (candidate.syanten_down)
            std::cout << " (向聴戻し)";
        std::cout << std::endl;

        // std::cout << "有効牌";
        // for (const auto [tile, n] : candidate.required_tiles)
        //     std::cout << fmt::format(" {}", Tile::Name.at(tile));
        // std::cout << std::endl;

        // if (exp_value_calculated) {
        //     std::cout << "巡目ごとの聴牌確率、和了確率、期待値" << std::endl;
        //     for (size_t i = 0; i < 17; ++i) {
        //         std::cout << fmt::format("{:>2}巡目 聴牌確率: {:>5.2f}%, 和了確率: "
        //                                  "{:>5.2f}%, 期待値: {:.2f}",
        //                                  i + 1, candidate.tenpai_probs[i] * 100,
        //                                  candidate.win_probs[i] * 100, candidate.exp_values[i])
        //                   << std::endl;
        //     }
        // }
    }
}
namespace py = pybind11;

PYBIND11_MODULE(mahjong_pythonlib_mod, m)
{
    m.doc() = "my test module";
    m.def("sample_mod1", &sample_mod1, "sample_mod1");
    py::enum_<Tile::Type>(m, "Tile")
        .value("Manzu1", Tile::Manzu1)
        .value("Manzu2", Tile::Manzu2)
        .value("Manzu3", Tile::Manzu3)
        .value("Manzu4", Tile::Manzu4)
        .value("Manzu5", Tile::Manzu5)
        .value("Manzu6", Tile::Manzu6)
        .value("Manzu7", Tile::Manzu7)
        .value("Manzu8", Tile::Manzu8)
        .value("Manzu9", Tile::Manzu9)
        .value("Pinzu1", Tile::Pinzu1)
        .value("Pinzu2", Tile::Pinzu2)
        .value("Pinzu3", Tile::Pinzu3)
        .value("Pinzu4", Tile::Pinzu4)
        .value("Pinzu5", Tile::Pinzu5)
        .value("Pinzu6", Tile::Pinzu6)
        .value("Pinzu7", Tile::Pinzu7)
        .value("Pinzu8", Tile::Pinzu8)
        .value("Pinzu9", Tile::Pinzu9)
        .value("Sozu1", Tile::Sozu1)
        .value("Sozu2", Tile::Sozu2)
        .value("Sozu3", Tile::Sozu3)
        .value("Sozu4", Tile::Sozu4)
        .value("Sozu5", Tile::Sozu5)
        .value("Sozu6", Tile::Sozu6)
        .value("Sozu7", Tile::Sozu7)
        .value("Sozu8", Tile::Sozu8)
        .value("Sozu9", Tile::Sozu9)
        .value("Ton", Tile::Ton)
        .value("Nan", Tile::Nan)
        .value("Sya", Tile::Sya)
        .value("Pe", Tile::Pe)
        .value("Haku", Tile::Haku)
        .value("Hatu", Tile::Hatu)
        .value("Tyun", Tile::Tyun)
        .value("AkaManzu5", Tile::AkaManzu5)
        .value("AkaPinzu5", Tile::AkaPinzu5)
        .value("AkaSozu5", Tile::AkaSozu5);

}