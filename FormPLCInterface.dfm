object Form_PLCInterface: TForm_PLCInterface
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'PLC Interface'
  ClientHeight = 861
  ClientWidth = 1297
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label4: TLabel
    Left = 359
    Top = 63
    Width = 40
    Height = 13
    Caption = 'Cell Info'
  end
  object GroupBox_PLC_PC: TGroupBox
    Left = -1
    Top = 50
    Width = 1290
    Height = 814
    Caption = 'PLC - PC INTERFACE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnDblClick = GroupBox_PLC_PCDblClick
    object ListView_PLC: TListView
      Left = 16
      Top = 32
      Width = 630
      Height = 769
      Columns = <
        item
          Caption = 'PLC_Address'
          Width = 120
        end
        item
          Caption = 'PLC_Name'
          Width = 200
        end
        item
          Caption = 'PLC_Value'
          Width = 290
        end>
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      GridLines = True
      ReadOnly = True
      RowSelect = True
      ParentFont = False
      TabOrder = 0
      ViewStyle = vsReport
    end
    object ListView_PC: TListView
      Left = 652
      Top = 32
      Width = 630
      Height = 769
      Columns = <
        item
          Caption = 'PC_Address'
          Width = 116
        end
        item
          Caption = 'PC_Name'
          Width = 200
        end
        item
          Caption = 'PC_Value'
          Width = 290
        end>
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      GridLines = True
      ReadOnly = True
      RowSelect = True
      ParentFont = False
      TabOrder = 1
      ViewStyle = vsReport
    end
    object chkAllData: TCheckBox
      Left = 1192
      Top = 11
      Width = 89
      Height = 17
      Caption = 'All Data'
      TabOrder = 2
      OnClick = chkAllDataClick
    end
  end
  object AdvSmoothButton_Close: TAdvSmoothButton
    Left = 1181
    Top = 4
    Width = 100
    Height = 40
    Appearance.Font.Charset = DEFAULT_CHARSET
    Appearance.Font.Color = clBlack
    Appearance.Font.Height = -15
    Appearance.Font.Name = 'Tahoma'
    Appearance.Font.Style = [fsBold]
    Appearance.Rounding = 3
    Status.Caption = '0'
    Status.Appearance.Fill.Color = clRed
    Status.Appearance.Fill.ColorMirror = clNone
    Status.Appearance.Fill.ColorMirrorTo = clNone
    Status.Appearance.Fill.GradientType = gtSolid
    Status.Appearance.Fill.GradientMirrorType = gtSolid
    Status.Appearance.Fill.BorderColor = clGray
    Status.Appearance.Fill.Rounding = 8
    Status.Appearance.Fill.ShadowOffset = 0
    Status.Appearance.Fill.Glow = gmNone
    Status.Appearance.Font.Charset = DEFAULT_CHARSET
    Status.Appearance.Font.Color = clWhite
    Status.Appearance.Font.Height = -11
    Status.Appearance.Font.Name = 'Tahoma'
    Status.Appearance.Font.Style = []
    BevelColor = clSilver
    Caption = 'Close'
    Color = 14935011
    ParentFont = False
    TabOrder = 1
    Version = '2.1.1.5'
    OnClick = AdvSmoothButton_CloseClick
    TMSStyle = 8
  end
  object Panel1: TAdvSmoothPanel
    Left = 288
    Top = 183
    Width = 809
    Height = 330
    Cursor = crDefault
    CanMove = True
    Caption.Location = plCenterCenter
    Caption.HTMLFont.Charset = DEFAULT_CHARSET
    Caption.HTMLFont.Color = clWindowText
    Caption.HTMLFont.Height = -11
    Caption.HTMLFont.Name = 'Tahoma'
    Caption.HTMLFont.Style = []
    Caption.HTMLURLColor = clBlack
    Caption.Font.Charset = DEFAULT_CHARSET
    Caption.Font.Color = clWindowText
    Caption.Font.Height = -24
    Caption.Font.Name = 'Tahoma'
    Caption.Font.Style = [fsBold]
    Caption.ColorStart = clWhite
    Caption.ColorEnd = clWhite
    Caption.LineColor = clBlack
    Caption.Line = False
    Caption.TextAlignment = taCenter
    Fill.Color = clWhite
    Fill.ColorTo = clWhite
    Fill.ColorMirror = clWhite
    Fill.ColorMirrorTo = clWhite
    Fill.GradientType = gtVertical
    Fill.GradientMirrorType = gtVertical
    Fill.BorderColor = 14922381
    Fill.Rounding = 5
    Fill.ShadowOffset = 0
    Fill.Glow = gmNone
    Version = '1.5.2.1'
    Visible = False
    TabOrder = 2
    TMSStyle = 0
    object lblConfiguration: TLabel
      Left = 7
      Top = 7
      Width = 78
      Height = 19
      Caption = 'PLC TEST'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = [fsBold, fsUnderline]
      ParentFont = False
    end
    object btnSaveConfig: TAdvSmoothButton
      Left = 323
      Top = 492
      Width = 80
      Height = 40
      Appearance.Font.Charset = DEFAULT_CHARSET
      Appearance.Font.Color = clWindowText
      Appearance.Font.Height = -17
      Appearance.Font.Name = 'Tahoma'
      Appearance.Font.Style = [fsBold]
      Appearance.Spacing = 0
      Appearance.Rounding = 3
      Appearance.WordWrapping = False
      Status.Caption = '0'
      Status.Appearance.Fill.Color = clRed
      Status.Appearance.Fill.ColorMirror = clNone
      Status.Appearance.Fill.ColorMirrorTo = clNone
      Status.Appearance.Fill.GradientType = gtSolid
      Status.Appearance.Fill.GradientMirrorType = gtSolid
      Status.Appearance.Fill.BorderColor = clGray
      Status.Appearance.Fill.Rounding = 0
      Status.Appearance.Fill.ShadowOffset = 0
      Status.Appearance.Fill.Glow = gmNone
      Status.Appearance.Font.Charset = DEFAULT_CHARSET
      Status.Appearance.Font.Color = clWhite
      Status.Appearance.Font.Height = -11
      Status.Appearance.Font.Name = 'Tahoma'
      Status.Appearance.Font.Style = []
      BevelColor = clMedGray
      Caption = 'Save '
      Color = clWhite
      ParentFont = False
      TabOrder = 0
      Visible = False
      Version = '2.1.1.5'
      TMSStyle = 8
    end
    object pnlCurrMin: TPanel
      Left = 13
      Top = 518
      Width = 184
      Height = 24
      Margins.Top = 5
      Margins.Bottom = 5
      BevelKind = bkFlat
      BevelOuter = bvNone
      Caption = 'PreCharging NG - Min. (mA)'
      Color = 15658734
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      Padding.Top = 5
      ParentBackground = False
      ParentFont = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 1
      Visible = False
    end
    object editCurrMin: TEdit
      Left = 199
      Top = 518
      Width = 108
      Height = 24
      Margins.Bottom = 0
      Alignment = taCenter
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      Text = '50'
      Visible = False
    end
    object GroupBox3: TGroupBox
      Left = 31
      Top = 47
      Width = 434
      Height = 135
      Caption = 'Write Value'
      TabOrder = 3
      object Label2: TLabel
        Left = 16
        Top = 22
        Width = 55
        Height = 16
        Caption = 'ADDRESS'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label3: TLabel
        Left = 16
        Top = 75
        Width = 37
        Height = 16
        Caption = 'VALUE'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object cbAddress: TComboBox
        Left = 16
        Top = 44
        Width = 225
        Height = 21
        TabOrder = 0
        Text = '38000'
        Items.Strings = (
          '38000'
          '38001'
          '38002'
          '38003'
          '38004'
          '38005'
          '38006'
          '38007'
          '38010'
          '38011'
          '38013'
          '38015'
          '38017'
          '')
      end
      object editValue: TEdit
        Left = 16
        Top = 98
        Width = 225
        Height = 21
        TabOrder = 1
        Text = '1'
      end
      object btnWriteValue: TButton
        Left = 247
        Top = 56
        Width = 83
        Height = 54
        Caption = 'Write Value'
        TabOrder = 2
        OnClick = btnWriteValueClick
      end
    end
    object GroupBox2: TGroupBox
      Left = 31
      Top = 188
      Width = 434
      Height = 112
      Caption = 'IROCV NG Value'
      TabOrder = 4
      object editIrOcvNg: TEdit
        Left = 16
        Top = 30
        Width = 401
        Height = 21
        TabOrder = 0
        Text = 
          '1,22,43,64,85,106,127,148,169,190,211,232,253,274,295,316,337,35' +
          '8,379,400'
      end
      object btnWriteNgValue: TButton
        Left = 304
        Top = 65
        Width = 113
        Height = 40
        Caption = 'Write NG Value'
        TabOrder = 1
        OnClick = btnWriteNgValueClick
      end
    end
    object GroupBox1: TGroupBox
      Left = 496
      Top = 47
      Width = 257
      Height = 250
      Caption = 'IR/OCV Value'
      TabOrder = 5
      object Label5: TLabel
        Left = 169
        Top = 40
        Width = 48
        Height = 16
        Caption = 'IR Value'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 156
        Top = 104
        Width = 61
        Height = 16
        Caption = 'OCV Value'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object editIR: TEdit
        Left = 39
        Top = 62
        Width = 178
        Height = 21
        TabOrder = 0
        Text = '16.5'
      end
      object editOCV: TEdit
        Left = 39
        Top = 126
        Width = 178
        Height = 21
        TabOrder = 1
        Text = '3100.5'
      end
      object btnWriteIrOcvValue: TButton
        Left = 104
        Top = 161
        Width = 113
        Height = 40
        Caption = 'Write IROCV Value'
        TabOrder = 2
        OnClick = btnWriteIrOcvValueClick
      end
    end
  end
  object Timer_Update: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer_UpdateTimer
    Left = 258
    Top = 24
  end
end
