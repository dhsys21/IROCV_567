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
      'STATUS='#49345#53468
      'PROCESS='#51652#54665
      'CHANNEL='#52292#45328
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO='#48520#47049#51221#48372
      'INIT='#52488#44592#54868
      'OK='#54869#51064
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
      'STATUS=STATUS'
      'PROCESS=PROCESS'
      'CHANNEL=CHANNEL'
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO=NG INFO.'
      'INIT=INIT.'
      'OK=OK'
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
    Height = 265
    Strings.Strings = (
      'SAVE='#20445#23384
      'CANCEL='#20445#23384
      'CONFIG='#37197#32622
      'MANUAL='#25163#21160
      'AUTO='#33258#21160
      'RESET='#37325#32622
      'TRAYOUT= '#25176#30424#21462#20986
      'TRAYID='#25176#30424'ID'
      'STATUS='#29366#24577
      'PROCESS='#36807#31243
      'CHANNEL='#36890#36947
      'IR=IR'
      'OCV=OCV'
      'REMEAINFO='#19981#33391#20449#24687
      'INIT='#21021#22987#21270
      'OK='#30830#23450
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
      18)
  end
end
