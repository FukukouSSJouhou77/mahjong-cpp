#include "../mahjong/mahjong.hpp"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>

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

        std::cout << fmt::format("[打 {}] 有効牌: {:>2d}種{:>2d}枚", Tile::EnName.at(candidate.tile),
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
    return 43044;
}
std::vector<Candidate> sort_Candicate(std::vector<Candidate> &candidates, int turn)
{

    std::sort(candidates.begin(), candidates.end(), [turn](const Candidate &a, const Candidate &b) {
        return a.exp_values[turn - 1] > b.exp_values[turn - 1];
    });
    return candidates;
}
int accumulate_candicate(Candidate &candidate)
{
    return std::accumulate(candidate.required_tiles.begin(), candidate.required_tiles.end(), 0,
                           [](auto &a, auto &b) { return a + std::get<1>(b); });
}
std::string strtest() { 
    return "Extra";
}
std::string get_NameEnTile(int tl) { 
    return Tile::EnName.at(tl);
}
namespace py = pybind11;

PYBIND11_MODULE(mahjong_pythonlib_mod, m)
{
    m.doc() = "my test module";
    m.def("sample_mod1", &sample_mod1, "sample_mod1");
    m.def("sort_Candicate", &sort_Candicate);
    m.def("accumulate_candicate", &accumulate_candicate, "accumulate_candicate");
    m.def("get_NameEnTile", &get_NameEnTile, "get_NameEnTile");
    m.def("strtest", &strtest, "strtest");
    pybind11::class_<Tile> tile(m, "Tile");
    py::enum_<Tile::Type>(tile, "Type")
        .value("Manzu1", Tile::Type::Manzu1)
        .value("Manzu2", Tile::Type::Manzu2)
        .value("Manzu3", Tile::Type::Manzu3)
        .value("Manzu4", Tile::Type::Manzu4)
        .value("Manzu5", Tile::Type::Manzu5)
        .value("Manzu6", Tile::Type::Manzu6)
        .value("Manzu7", Tile::Type::Manzu7)
        .value("Manzu8", Tile::Type::Manzu8)
        .value("Manzu9", Tile::Type::Manzu9)
        .value("Pinzu1", Tile::Type::Pinzu1)
        .value("Pinzu2", Tile::Type::Pinzu2)
        .value("Pinzu3", Tile::Type::Pinzu3)
        .value("Pinzu4", Tile::Type::Pinzu4)
        .value("Pinzu5", Tile::Type::Pinzu5)
        .value("Pinzu6", Tile::Type::Pinzu6)
        .value("Pinzu7", Tile::Type::Pinzu7)
        .value("Pinzu8", Tile::Type::Pinzu8)
        .value("Pinzu9", Tile::Type::Pinzu9)
        .value("Sozu1", Tile::Type::Sozu1)
        .value("Sozu2", Tile::Type::Sozu2)
        .value("Sozu3", Tile::Type::Sozu3)
        .value("Sozu4", Tile::Type::Sozu4)
        .value("Sozu5", Tile::Type::Sozu5)
        .value("Sozu6", Tile::Type::Sozu6)
        .value("Sozu7", Tile::Type::Sozu7)
        .value("Sozu8", Tile::Type::Sozu8)
        .value("Sozu9", Tile::Type::Sozu9)
        .value("Ton", Tile::Type::Ton)
        .value("Nan", Tile::Type::Nan)
        .value("Sya", Tile::Type::Sya)
        .value("Pe", Tile::Type::Pe)
        .value("Haku", Tile::Type::Haku)
        .value("Hatu", Tile::Type::Hatu)
        .value("Tyun", Tile::Type::Tyun)
        .value("AkaManzu5", Tile::Type::AkaManzu5)
        .value("AkaPinzu5", Tile::Type::AkaPinzu5)
        .value("AkaSozu5", Tile::Type::AkaSozu5)
        .export_values();
    py::class_<Hand>(m, "Hand").def(py::init<const std::vector<int>>())
        .def("to_string",&Hand::to_string);
    pybind11::class_<ExpectedValueCalculator> exvc(m, "ExpectedValueCalculator");
    exvc.def(py::init())
        .def("calc1",&ExpectedValueCalculator::calc1);
    pybind11::class_<ScoreCalculator> sc(m, "ScoreCalculator");
    sc.def(py::init())
        .def("set_bakaze",&ScoreCalculator::set_bakaze)
        .def("set_zikaze",&ScoreCalculator::set_zikaze)
        .def("set_dora_indicators",&ScoreCalculator::set_dora_indicators)
        .def("calc",&ScoreCalculator::calc);
    py::enum_<ExpectedValueCalculator::Flag>(exvc, "Flag")
        .value("Null", ExpectedValueCalculator::Flag::Null)
        .value("CalcSyantenDown", ExpectedValueCalculator::Flag::CalcSyantenDown)
        .value("CalcTegawari", ExpectedValueCalculator::Flag::CalcTegawari)
        .value("CalcDoubleReach", ExpectedValueCalculator::Flag::CalcDoubleReach)
        .value("CalcIppatu", ExpectedValueCalculator::Flag::CalcIppatu)
        .value("CalcHaiteitumo", ExpectedValueCalculator::Flag::CalcHaiteitumo)
        .value("CalcUradora", ExpectedValueCalculator::Flag::CalcUradora)
        .value("CalcAkaTileTumo", ExpectedValueCalculator::Flag::CalcAkaTileTumo)
        .value("MaximaizeWinProb", ExpectedValueCalculator::Flag::MaximaizeWinProb)
        .export_values();
    py::class_<Candidate> candicate_Z(m, "Candidate");
    candicate_Z.def_readwrite("tile", &Candidate::tile);
    candicate_Z.def_readwrite("tenpai_probs", &Candidate::tenpai_probs)
        .def_readwrite("exp_values",&Candidate::exp_values)
        .def_readwrite("tile",&Candidate::tile)
        .def_readwrite("win_probs",&Candidate::win_probs)
        .def_readwrite("required_tiles",&Candidate::required_tiles)
        .def_readwrite("syanten_down", &Candidate::syanten_down);
    candicate_Z.def_readwrite("required_tiles", &Candidate::required_tiles)
    pybind11::class_<SyantenCalculator> Syantenc(m, "SyantenCalculator");
    Syantenc.def(py::init())
        .def_static("calc",&SyantenCalculator::calc);


    
}