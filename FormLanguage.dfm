object Form_Language: TForm_Language
  Left = 0
  Top = 0
  Caption = 'Form_Language'
  ClientHeight = 783
  ClientWidth = 1019
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object VLE_KOREAN: TValueListEditor
    Left = 32
    Top = 24
    Width = 321
    Height = 265
    Strings.Strings = (
      'SAVE='#51200#51109
      'CANCEL='#52712#49548
      'CONFIG='#49444#51221
      'MANUAL='#49688#46041
      'AUTO='#51088#46041
      'RESET='#47532#49483
      'TRAYOUT='#53944#47112#51060#48176#52636
      'TRAYID='#53944#47112#51060#50500#51060#46356
      'TRAYINFO='#53944#47112#51060#51221#48372
      'PROCESSINFO=IR/OCV '#51652#54665#51221#48372
      'STATUS='#49345#53468
      'PROCESS='#51652#54665
      'CHANNEL='#52292#45328
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO='#51116#52769#51221#51221#48372
      'REMEASUREALARM='#51116#52769#51221#50508#46988
      'INIT='#52488#44592#54868
      'OK='#54869#51064
      'MSA=MSA'
      'CALIBRATION='#48372#51221
      'READDY='#45824#44592
      'IRCOMPLETE=IR'
      'OCVCOMPLETE=OCV'
      'IROCV=IR/OCV'
      'OCVFAIL=OCV '#48520#47049
      'IRFAIL=IR '#48520#47049
      'AVGFAIL=Avg. '#48520#47049
      'FAIL='#48520#47049
      'OUTFLOW=CELL '#50976#52636
      'NOCELL='#49472#50630#51020
      'CHANNELINFO='#52292#45328#51221#48372
      'POSITION='#50948#52824
      'START='#49884#51089
      'STOP='#51221#51648
      'EACHCHANNEL='#44060#48324#52292#45328
      'PROBESETTING='#54532#47196#48652#49444#51221
      'OPEN='#50676#44592
      'CLOSE='#45803#44592
      'msgTooManyNG=NG '#49472#51060' '#45320#47924' '#47566#49845#45768#45796'. '#54869#51064#54644#51452#49464#50836)
    TabOrder = 0
    ColWidths = (
      150
      148)
    RowHeights = (
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18)
  end
  object VLE_ENGLISH: TValueListEditor
    Left = 32
    Top = 312
    Width = 321
    Height = 265
    Strings.Strings = (
      'SAVE=SAVE'
      'CANCEL=CANCEL'
      'CONFIG=CONFIG'
      'MANUAL=MANUAL'
      'AUTO=AUTO'
      'RESET=RESET'
      'TRAYOUT=TRAY OUT'
      'TRAYID=TRAY ID'
      'TRAYINFO=TRAY INFO'
      'PROCESSINFO=IR/OCV PROCESS INFO'
      'STATUS=STATUS'
      'PROCESS=PROCESS'
      'CHANNEL=CHANNEL'
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO=NG INFO'
      'REMEASUREALARM=REMEASURE ALARM'
      'INIT=INIT.'
      'OK=OK'
      'MSA=MSA'
      'CALIBRATION=CALIBRATION'
      'READY=READY'
      'IRCOMPLETE=IR'
      'OCVCOMPLETE=OCV'
      'IROCV=IR/OCV'
      'OCVFAIL=OCV FAIL'
      'IRFAIL=IR FAIL'
      'AVGFAIL=Avg. FAIL'
      'FAIL=FAIL'
      'OUTFLOW=OUTFLOW'
      'NOCELL=NO CELL'
      'CHANNELINFO=CHANNEL INFO'
      'POSITION=POS'
      'START=START'
      'STOP=STOP'
      'EACHCHANNEL=EACH CHANNEL'
      'PROBESETTING=PROBE SETTING'
      'OPEN=OPEN'
      'CLOSE=CLOSE'
      'msgTooManyNG=Too many NG cells. Please check it.')
    TabOrder = 1
    ColWidths = (
      150
      148)
    RowHeights = (
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18)
  end
  object sGridLanguage: TStringGrid
    Left = 770
    Top = 552
    Width = 231
    Height = 217
    ColCount = 3
    RowCount = 30
    TabOrder = 2
    ColWidths = (
      64
      64
      64)
    RowHeights = (
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24
      24)
  end
  object VLE_CHINESE: TValueListEditor
    Left = 400
    Top = 24
    Width = 321
    Height = 553
    Strings.Strings = (
      'SAVE='#20445#23384
      'CANCEL='#30830#35748
      'CONFIG='#37197#32622
      'MANUAL='#25163#21160
      'AUTO='#33258#21160
      'RESET='#37325#32622
      'TRAYOUT= '#25176#30424#21462#20986
      'TRAYID='#25176#30424'ID'
      'TRAYINFO='#25176#30424#24773#22577
      'PROCESSINFO=IR/OCV '#36807#31243#24773#22577
      'STATUS='#29366#24577
      'PROCESS='#36807#31243
      'CHANNEL='#36890#36947
      'IR=IR'
      'OCV=OCV'
      'CONNECT='#36830#25509
      'DISCONNECT='#26029#24320
      'REMEAINFO='#19981#33391#20449#24687
      'REMEASUREALARM='#19981#33391#25253#35686
      'INIT='#21021#22987#21270
      'OK='#30830#23450
      'MSA=MSA'
      'CALIBRATION='#26657#20934
      'READDY='#31561#24453
      'IRCOMPLETE=IR '#23436#25104
      'OCVCOMPLETE=OCV '#23436#25104
      'IROCV=IR/OCV'
      'OCVFAIL=OCV '#38169#35823
      'IRFAIL=IR '#38169#35823
      'AVGFAIL=Avg. '#38169#35823
      'FAIL='#38169#35823
      'OUTFLOW=CELL'#27969#20986
      'NOCELL='#26080' CELL'
      'CHANNELINFO='#36890#36947#24773#22577
      'POSITION='#20301#32622
      'START='#24320#22987
      'STOP='#20572#27490
      'EACHCHANNEL='#21508#36890#36947
      'PROBESETTING='#25506#22836#35774#32622
      'OPEN='#25171#24320
      'CLOSE='#20851#38381
      'msgTooManyNG= '#19981#33391#21697#36807#22810#65292#35831#26816#26597#12290)
    TabOrder = 3
    ColWidths = (
      150
      148)
    RowHeights = (
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18
      18)
  end
end
