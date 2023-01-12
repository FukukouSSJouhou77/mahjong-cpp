#include "../mahjong/mahjong.hpp"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <pybind11/pybind11.h>
using namespace mahjong;
int sample_mod1() {

    // ���v�̎�v
    Hand hand1({Tile::Manzu2, Tile::Manzu2, Tile::Manzu2, Tile::Manzu5, Tile::Manzu6, Tile::Manzu7,
                Tile::Pinzu3, Tile::Pinzu4, Tile::Pinzu5, Tile::Sozu3, Tile::Sozu3, Tile::Sozu6,
                Tile::Sozu6, Tile::Sozu7});
    // 1�����̎�v
    Hand hand2({Tile::Manzu2, Tile::Manzu2, Tile::Manzu2, Tile::Manzu5, Tile::Manzu6, Tile::Manzu7,
                Tile::Pinzu3, Tile::Pinzu4, Tile::Sozu3, Tile::Sozu3, Tile::Sozu6, Tile::Sozu6,
                Tile::Sozu7, Tile::Pe});
    // 3�����̎�v
    Hand hand3({Tile::Manzu1, Tile::Manzu1, Tile::Manzu2, Tile::Manzu4, Tile::Manzu5, Tile::Manzu7,
                Tile::Pinzu9, Tile::Sozu3, Tile::Sozu7, Tile::Sozu9, Tile::Ton, Tile::Pe, Tile::Pe,
                Tile::Hatu});
    // 2�����̎�v
    Hand hand4({Tile::Manzu1, Tile::Manzu2, Tile::Manzu3, Tile::Manzu3, Tile::Manzu4, Tile::Manzu9,
                Tile::Pinzu3, Tile::Pinzu6, Tile::Pinzu8, Tile::Pinzu8, Tile::Sozu1, Tile::Sozu2,
                Tile::Sozu4, Tile::Sozu5});
    // 1�����̎�v
    Hand hand5({Tile::Manzu1, Tile::Manzu2, Tile::Manzu4, Tile::Manzu5, Tile::Manzu5, Tile::Pinzu3,
                Tile::Pinzu4, Tile::Pinzu5, Tile::Pinzu6, Tile::Pinzu7, Tile::Sozu6, Tile::Sozu7,
                Tile::Sozu7, Tile::Sozu7});

    int bakaze = Tile::Ton;                 // �ꕗ
    int zikaze = Tile::Ton;                 // ����
    int turn = 3;                           // ����
    int syanten_type = SyantenType::Normal; // �������̎��
    // �l�����鍀��
    int flag = ExpectedValueCalculator::CalcSyantenDown   // �����߂��l��
               | ExpectedValueCalculator::CalcTegawari    // ��ς��l��
               | ExpectedValueCalculator::CalcDoubleReach // �_�u�������l��
               | ExpectedValueCalculator::CalcIppatu      // �ꔭ�l��
               | ExpectedValueCalculator::CalcHaiteitumo  // �C�ꝝ���l��
               | ExpectedValueCalculator::CalcUradora     // ���h���l��
               | ExpectedValueCalculator::CalcAkaTileTumo // �Ԕv���̍l��
        //| ExpectedValueCalculator::MaximaizeWinProb // �a���m�����ő剻
        ;
    std::vector<int> dora_indicators = {Tile::Ton}; // �h���\���v
    Hand hand = hand5;                              // ��v

    ExpectedValueCalculator exp_value_calculator;
    ScoreCalculator score_calculator;

    // �_���v�Z�̐ݒ�
    score_calculator.set_bakaze(bakaze);
    score_calculator.set_zikaze(zikaze);
    score_calculator.set_dora_indicators(dora_indicators);

    // ���������v�Z����B
    auto [_, syanten] = SyantenCalculator::calc(hand, syanten_type);

    // ���Ғl���v�Z����B
    auto begin = std::chrono::steady_clock::now();
    auto [success, candidates] =
        exp_value_calculator.calc(hand, score_calculator, dora_indicators, syanten_type, flag);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    if (!success) {
        std::cout << "�G���[" << std::endl;
        return 1;
    }

    bool exp_value_calculated = !candidates[0].tenpai_probs.empty();

    // ���Ғl���������Ƀ\�[�g����B
    if (exp_value_calculated)
        std::sort(candidates.begin(), candidates.end(),
                  [turn](const Candidate &a, const Candidate &b) {
                      return a.exp_values[turn - 1] > b.exp_values[turn - 1];
                  });

    // ���ʂ��o�͂���B
    ////////////////////////////////////////////////////////////////////////////////////
    std::cout << fmt::format("��v: {}, ������: {}, ����: {}", hand.to_string(), syanten, turn)
              << std::endl;
    std::cout << fmt::format("�v�Z����: {}us", elapsed_ms) << std::endl;

    for (const auto &candidate : candidates) {
        int sum_required_tiles =
            std::accumulate(candidate.required_tiles.begin(), candidate.required_tiles.end(), 0,
                            [](auto &a, auto &b) { return a + std::get<1>(b); });

        std::cout << fmt::format("[�� {}] �L���v: {:>2d}��{:>2d}��", Tile::Name.at(candidate.tile),
                                 candidate.required_tiles.size(), sum_required_tiles);

        if (exp_value_calculated)
            std::cout << fmt::format(", ���v�m��: {:>5.2f}%, �a���m��: {:>5.2f}%, ���Ғl: {:>7.2f}",
                                     candidate.tenpai_probs[turn - 1] * 100,
                                     candidate.win_probs[turn - 1] * 100,
                                     candidate.exp_values[turn - 1]);
        if (candidate.syanten_down)
            std::cout << " (�����߂�)";
        std::cout << std::endl;

        // std::cout << "�L���v";
        // for (const auto [tile, n] : candidate.required_tiles)
        //     std::cout << fmt::format(" {}", Tile::Name.at(tile));
        // std::cout << std::endl;

        // if (exp_value_calculated) {
        //     std::cout << "���ڂ��Ƃ̒��v�m���A�a���m���A���Ғl" << std::endl;
        //     for (size_t i = 0; i < 17; ++i) {
        //         std::cout << fmt::format("{:>2}���� ���v�m��: {:>5.2f}%, �a���m��: "
        //                                  "{:>5.2f}%, ���Ғl: {:.2f}",
        //                                  i + 1, candidate.tenpai_probs[i] * 100,
        //                                  candidate.win_probs[i] * 100, candidate.exp_values[i])
        //                   << std::endl;
        //     }
        // }
    }
    return 0;
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
    py::class_<Hand>(m, "Hand").def(py::init<const std::vector<int>>());


}