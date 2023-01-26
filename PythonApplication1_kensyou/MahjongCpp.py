
import mahjong_pythonlib_mod as pymod


def Intmahjong(x):
    if x==0:
        return pymod.Tile.Manzu1
    elif x==1:
        return pymod.Tile.Manzu2
    elif x==2:
        return pymod.Tile.Manzu3
    elif x==3:
        return pymod.Tile.Manzu4
    elif x==4:
        return pymod.Tile.Manzu5
    elif x==5:
        return pymod.Tile.Manzu6
    elif x==6:
        return pymod.Tile.Manzu7
    elif x==7:
        return pymod.Tile.Manzu8
    elif x==8:
        return pymod.Tile.Manzu9
    elif x==9:
        return pymod.Tile.Pinzu1
    elif x==10:
        return pymod.Tile.Pinzu2
    elif x==11:
        return pymod.Tile.Pinzu3
    elif x==12:
        return pymod.Tile.Pinzu4
    elif x==13:
        return pymod.Tile.Pinzu5
    elif x==14:
        return pymod.Tile.Pinzu6
    elif x==15:
        return pymod.Tile.Pinzu7
    elif x==16:
        return pymod.Tile.Pinzu8
    elif x==17:
        return pymod.Tile.Pinzu9
    elif x==18:
        return pymod.Tile.Sozu1
    elif x==19:
        return pymod.Tile.Sozu2
    elif x==20:
        return pymod.Tile.Sozu3
    elif x==21:
        return pymod.Tile.Sozu4
    elif x==22:
        return pymod.Tile.Sozu5
    elif x==23:
        return pymod.Tile.Sozu6
    elif x==24:
        return pymod.Tile.Sozu7
    elif x==25:
        return pymod.Tile.Sozu8
    elif x==26:
        return pymod.Tile.Sozu9
    elif x==27:
        return pymod.Tile.Ton
    elif x==28:
        return pymod.Tile.Nan
    elif x==29:
        return pymod.Tile.Sya
    elif x==30:
        return pymod.Tile.Pe
    elif x==31:
        return pymod.Tile.Haku
    elif x==32:
        return pymod.Tile.Hatu
    elif x==33:
        return pymod.Tile.Tyun
    elif x==34:
        return pymod.Tile.AkaManzu5
    elif x==35:
        return pymod.Tile.AkaPinzu5
    elif x==36:
        return pymod.Tile.AkaSozu5
    else:
        print("IntmahjongERR")
        exit()


def Mahjongint_OLD(x):
    if x=="1Man":
        return 0
    elif x=="2Man":
        return 1
    elif x=="3Man":
        return 2
    elif x=="4Man":
        return 3
    elif x=="5Man":
        return 4
    elif x=="6Man":
        return 5
    elif x=="7Man":
        return 6
    elif x=="8Man":
        return 7
    elif x=="9Man":
        return 8
    elif x=="1Pin":
        return 9
    elif x=="2Pin":
        return 10
    elif x=="3Pin":
        return 11
    elif x=="4Pin":
        return 12
    elif x=="5Pin":
        return 13
    elif x=="6Pin":
        return 14
    elif x=="7Pin":
        return 15
    elif x=="8Pin":
        return 16
    elif x=="9Pin":
        return 17
    elif x=="1So ":
        return 18
    elif x=="2So ":
        return 19
    elif x=="3So ":
        return 20
    elif x=="4So ":
        return 21
    elif x=="5So ":
        return 22
    elif x=="6So ":
        return 23
    elif x=="7So ":
        return 24
    elif x=="8So ":
        return 25
    elif x=="9So ":
        return 26
    elif x=="Ton ":
        return 27
    elif x=="Nan ":
        return 28
    elif x=="Sya ":
        return 29
    elif x=="Pe  ":
        return 30
    elif x=="Haku":
        return 31
    elif x=="Hatu":
        return 32
    elif x=="Tyun":
        return 33
    elif x=="5AM ":
        return 34
    elif x=="5AP ":
        return 35
    elif x=="5AS ":
        return 36
    else:
        print("MahjongintERR")
        print(type(x))
        print(len(x))
        exit()




def mahjongcpp(x,y):

    print("Python Module!")
    handkun=pymod.Hand([int(pymod.Tile.Manzu1)])
    hand1=pymod.Hand([int(Intmahjong(x[0])),
                      int(Intmahjong(x[1])),
                      int(Intmahjong(x[2])),
                      int(Intmahjong(x[3])),
                      int(Intmahjong(x[4])),
                      int(Intmahjong(x[5])),
                      int(Intmahjong(x[6])),
                      int(Intmahjong(x[7])),
                      int(Intmahjong(x[8])),
                      int(Intmahjong(x[9])),
                      int(Intmahjong(x[10])),
                      int(Intmahjong(x[11])),
                      int(Intmahjong(x[12])),
                      int(Intmahjong(x[13]))])

    bakaze=int(pymod.Tile.Ton);
    zikaze=int(pymod.Tile.Ton);
    turn=y;
    syanten_type=1;
    flag = int(pymod.ExpectedValueCalculator.CalcSyantenDown )| int(pymod.ExpectedValueCalculator.CalcTegawari   )| int(pymod.ExpectedValueCalculator.CalcDoubleReach )| int(pymod.ExpectedValueCalculator.CalcIppatu )| int(pymod.ExpectedValueCalculator.CalcHaiteitumo )| int(pymod.ExpectedValueCalculator.CalcUradora )| int(pymod.ExpectedValueCalculator.CalcAkaTileTumo)
    dora_indicators=[int(pymod.Tile.Ton)]
    hand=hand1
    exp_value_calculator=pymod.ExpectedValueCalculator()
    score_calculator=pymod.ScoreCalculator()
    score_calculator.set_bakaze(bakaze)
    score_calculator.set_zikaze(zikaze)
    score_calculator.set_dora_indicators(dora_indicators)
    c,syanten=pymod.SyantenCalculator.calc(hand,syanten_type)
    success, candidates =exp_value_calculator.calc1(hand, score_calculator, dora_indicators, syanten_type, flag)
    if success:
        None
    else:
        print("ERR")
        exit(-1)
        return "ERR"
    exp_value_calculated = candidates[0].tenpai_probs
    if exp_value_calculated:
        candidates=pymod.sort_Candicate(candidates,turn)
    # print("Tehai: %s, Syanten: %d, Turn: %d" % (hand.to_string(),syanten,turn))
    print(pymod.get_NameEnTile(int(candidates[0].tile)))
    return Mahjongint(candidates[0].tile)
    '''
    for candidate in candidates:
        sum_required_tiles=pymod.accumulate_candicate(candidate)
        print("[Da {}] Enabled hi : {:>2d} / {:>2d}".format(pymod.get_NameEnTile(int(candidate.tile)),len(candidate.required_tiles),sum_required_tiles),end="")
        if exp_value_calculated:
            print(" Tenpai Ex: {:>5.2f}%,Hora Ex: {:>5.2f}%,Ex: {:>7.2f}".format(candidate.tenpai_probs[turn - 1] * 100,candidate.win_probs[turn - 1] * 100,
                                         candidate.exp_values[turn - 1]),end="")
            if(candidate.syanten_down):
                print(" (Syanten Back)",end="")
        print("")
    print("test")
    print(pymod.strtest())
    input()'''


def Mahjongint(x):
    if x == pymod.Tile.Manzu1:
        return 0
    elif x == pymod.Tile.Manzu2:
        return 1
    elif x == pymod.Tile.Manzu3:
        return 2
    elif x == pymod.Tile.Manzu4:
        return 3
    elif x == pymod.Tile.Manzu5:
        return 4
    elif x == pymod.Tile.Manzu6:
        return 5
    elif x == pymod.Tile.Manzu7:
        return 6
    elif x == pymod.Tile.Manzu8:
        return 7
    elif x == pymod.Tile.Manzu9:
        return 8
    elif x == pymod.Tile.Pinzu1:
        return 9
    elif x == pymod.Tile.Pinzu2:
        return 10
    elif x == pymod.Tile.Pinzu3:
        return 11
    elif x == pymod.Tile.Pinzu4:
        return 12
    elif x == pymod.Tile.Pinzu5:
        return 13
    elif x == pymod.Tile.Pinzu6:
        return 14
    elif x == pymod.Tile.Pinzu7:
        return 15
    elif x == pymod.Tile.Pinzu8:
        return 16
    elif x == pymod.Tile.Pinzu9:
        return 17
    elif x == pymod.Tile.Sozu1:
        return 18
    elif x == pymod.Tile.Sozu2:
        return 19
    elif x == pymod.Tile.Sozu3:
        return 20
    elif x == pymod.Tile.Sozu4:
        return 21
    elif x == pymod.Tile.Sozu5:
        return 22
    elif x == pymod.Tile.Sozu6:
        return 23
    elif x == pymod.Tile.Sozu7:
        return 24
    elif x == pymod.Tile.Sozu8:
        return 25
    elif x == pymod.Tile.Sozu9:
        return 26
    elif x == pymod.Tile.Ton:
        return 27
    elif x == pymod.Tile.Nan:
        return 28
    elif x == pymod.Tile.Sya:
        return 29
    elif x == pymod.Tile.Pe:
        return 30
    elif x == pymod.Tile.Haku:
        return 31
    elif x == pymod.Tile.Hatu:
        return 32
    elif x == pymod.Tile.Tyun:
        return 33
    elif x == pymod.Tile.AkaManzu5:
        return 34
    elif x == pymod.Tile.AkaPinzu5:
        return 35
    elif x == pymod.Tile.AkaSozu5:
        return 36
    else:
        return -1