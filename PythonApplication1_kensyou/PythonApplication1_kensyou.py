
import mahjong_pythonlib_mod as pymod
a=pymod.sample_mod1()
handkun=pymod.Hand([int(pymod.Tile.Manzu1)])
hand1=pymod.Hand([int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu5), int(pymod.Tile.Manzu6), int(pymod.Tile.Manzu7),int(pymod.Tile.Pinzu3), int(pymod.Tile.Pinzu4), int(pymod.Tile.Pinzu5), int(pymod.Tile.Sozu3), int(pymod.Tile.Sozu3), int(pymod.Tile.Sozu6),int(pymod.Tile.Sozu6), int(pymod.Tile.Sozu7)])
hand2=pymod.Hand([int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu5), int(pymod.Tile.Manzu6), int(pymod.Tile.Manzu7),int(pymod.Tile.Pinzu3), int(pymod.Tile.Pinzu4), int(pymod.Tile.Sozu3), int(pymod.Tile.Sozu3), int(pymod.Tile.Sozu6), int(pymod.Tile.Sozu6),int(pymod.Tile.Sozu7), int(pymod.Tile.Pe)])
hand3=pymod.Hand([int(pymod.Tile.Manzu1), int(pymod.Tile.Manzu1), int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu4), int(pymod.Tile.Manzu5), int(pymod.Tile.Manzu7),
                int(pymod.Tile.Pinzu9), int(pymod.Tile.Sozu3), int(pymod.Tile.Sozu7), int(pymod.Tile.Sozu9), int(pymod.Tile.Ton), int(pymod.Tile.Pe), int(pymod.Tile.Pe),
                int(pymod.Tile.Hatu)]);
hand4=pymod.Hand([int(pymod.Tile.Manzu1), int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu3), int(pymod.Tile.Manzu3), int(pymod.Tile.Manzu4), int(pymod.Tile.Manzu9),
                int(pymod.Tile.Pinzu3), int(pymod.Tile.Pinzu6), int(pymod.Tile.Pinzu8), int(pymod.Tile.Pinzu8), int(pymod.Tile.Sozu1), int(pymod.Tile.Sozu2),
                int(pymod.Tile.Sozu4), int(pymod.Tile.Sozu5)]);
hand5=pymod.Hand([int(pymod.Tile.Manzu1), int(pymod.Tile.Manzu2), int(pymod.Tile.Manzu4), int(pymod.Tile.Manzu5), int(pymod.Tile.Manzu5), int(pymod.Tile.Pinzu3),
                int(pymod.Tile.Pinzu4), int(pymod.Tile.Pinzu5), int(pymod.Tile.Pinzu6), int(pymod.Tile.Pinzu7), int(pymod.Tile.Sozu6), int(pymod.Tile.Sozu7),
                int(pymod.Tile.Sozu7), int(pymod.Tile.Sozu7)]);
bakaze=int(pymod.Tile.Ton);
zikaze=int(pymod.Tile.Ton);
turn=3;
syanten_type=1;
flag = int(pymod.ExpectedValueCalculator.CalcSyantenDown )| int(pymod.ExpectedValueCalculator.CalcTegawari   )| int(pymod.ExpectedValueCalculator.CalcDoubleReach )| int(pymod.ExpectedValueCalculator.CalcIppatu )| int(pymod.ExpectedValueCalculator.CalcHaiteitumo )| int(pymod.ExpectedValueCalculator.CalcUradora )| int(pymod.ExpectedValueCalculator.CalcAkaTileTumo)
dora_indicators=[int(pymod.Tile.Ton)]
hand=hand5
exp_value_calculator=pymod.ExpectedValueCalculator()
score_calculator=pymod.ScoreCalculator()
score_calculator.set_bakaze(bakaze)
score_calculator.set_zikaze(zikaze)
score_calculator.set_dora_indicators(dora_indicators)
print("test")
print(a)
input()