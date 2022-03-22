object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'FlexFarm v0.1'
  ClientHeight = 395
  ClientWidth = 633
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 16
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 633
    Height = 395
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 0
    OnExit = PageControl1Exit
    object TabSheet1: TTabSheet
      Caption = 'Bots'
      object Label16: TLabel
        Left = 186
        Top = 8
        Width = 107
        Height = 18
        Caption = 'No bot selected.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 177
        Height = 364
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object Button2: TButton
          Left = 83
          Top = 336
          Width = 92
          Height = 25
          Caption = 'Remove'
          TabOrder = 0
          OnClick = Button2Click
        end
        object CheckBox1: TCheckBox
          Left = 5
          Top = 0
          Width = 173
          Height = 34
          Caption = 'Select All'
          TabOrder = 1
        end
        object Edit1: TEdit
          Left = 2
          Top = 279
          Width = 173
          Height = 24
          TabOrder = 2
          TextHint = 'GrowID'
        end
        object Edit2: TEdit
          Left = 2
          Top = 306
          Width = 173
          Height = 24
          PasswordChar = '*'
          TabOrder = 3
          TextHint = 'Password'
        end
        object ListBox1: TListBox
          Left = 2
          Top = 32
          Width = 173
          Height = 241
          MultiSelect = True
          PopupMenu = PopupMenu1
          TabOrder = 4
          OnKeyUp = ListBox1KeyUp
          OnMouseDown = ListBox1MouseDown
        end
        object Button1: TButton
          Left = 2
          Top = 336
          Width = 75
          Height = 25
          Caption = 'Add'
          TabOrder = 5
          OnClick = Button1Click
        end
      end
      object PageControl2: TPageControl
        Left = 177
        Top = 0
        Width = 448
        Height = 364
        ActivePage = Control
        Align = alClient
        TabOrder = 1
        Visible = False
        object Control: TTabSheet
          Caption = 'Control'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          object CheckBox3: TCheckBox
            Left = 19
            Top = 35
            Width = 62
            Height = 17
            Caption = 'Follow'
            TabOrder = 0
            OnClick = CheckBox3Click
          end
          object CheckBox2: TCheckBox
            Left = 19
            Top = 12
            Width = 134
            Height = 17
            Caption = 'Auto Reconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = CheckBox2Click
          end
          object Edit3: TEdit
            Left = 100
            Top = 35
            Width = 173
            Height = 26
            TabOrder = 2
            TextHint = 'Name'
            OnChange = Edit3Change
          end
          object Edit4: TEdit
            Left = 100
            Top = 67
            Width = 173
            Height = 26
            TabOrder = 3
            TextHint = 'World'
          end
          object Button8: TButton
            Left = 279
            Top = 58
            Width = 49
            Height = 43
            Caption = #8592
            TabOrder = 4
            OnClick = Button8Click
          end
          object Button5: TButton
            Left = 388
            Top = 58
            Width = 49
            Height = 43
            Caption = #8594
            TabOrder = 5
            OnClick = Button5Click
          end
          object Button6: TButton
            Left = 334
            Top = 107
            Width = 49
            Height = 43
            Caption = #8595
            TabOrder = 6
            OnClick = Button6Click
          end
          object Button4: TButton
            Left = 335
            Top = 9
            Width = 49
            Height = 43
            Caption = #8593
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            OnClick = Button4Click
          end
          object Button3: TButton
            Left = 19
            Top = 67
            Width = 75
            Height = 25
            Caption = 'Warp'
            TabOrder = 8
            OnClick = Button3Click
          end
          object CheckBox7: TCheckBox
            Left = 19
            Top = 99
            Width = 105
            Height = 17
            Caption = 'Auto access'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
            OnClick = CheckBox7Click
          end
          object GroupBox1: TGroupBox
            Left = 19
            Top = 168
            Width = 254
            Height = 145
            Caption = 'Info'
            TabOrder = 10
            object Label8: TLabel
              Left = 16
              Top = 24
              Width = 49
              Height = 18
              Caption = 'Name: '
            end
            object Label9: TLabel
              Left = 16
              Top = 46
              Width = 33
              Height = 18
              Caption = 'level:'
            end
            object Label10: TLabel
              Left = 16
              Top = 68
              Width = 43
              Height = 18
              Caption = 'Gems:'
            end
            object Label11: TLabel
              Left = 16
              Top = 90
              Width = 55
              Height = 18
              Caption = 'User id: '
            end
            object Label12: TLabel
              Left = 16
              Top = 112
              Width = 141
              Height = 18
              Caption = 'World player amount:'
            end
          end
          object Button33: TButton
            Left = 334
            Top = 58
            Width = 49
            Height = 43
            Caption = 'Use'
            TabOrder = 11
            OnClick = Button33Click
          end
          object CheckBox11: TCheckBox
            Left = 19
            Top = 122
            Width = 126
            Height = 28
            Caption = 'Enable Collecing'
            Checked = True
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            State = cbChecked
            TabOrder = 12
            OnClick = CheckBox11Click
          end
        end
        object TabSheet3: TTabSheet
          Caption = 'Automation'
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Label1: TLabel
            Left = 162
            Top = 5
            Width = 32
            Height = 16
            Caption = 'Tiles:'
          end
          object Button9: TButton
            Left = 272
            Top = 26
            Width = 49
            Height = 32
            TabOrder = 0
            OnClick = Button9Click
          end
          object Button10: TButton
            Left = 327
            Top = 26
            Width = 49
            Height = 32
            TabOrder = 1
            OnClick = Button10Click
          end
          object Button11: TButton
            Left = 327
            Top = 64
            Width = 49
            Height = 32
            TabOrder = 2
            OnClick = Button11Click
          end
          object Button12: TButton
            Left = 272
            Top = 64
            Width = 49
            Height = 32
            TabOrder = 3
            OnClick = Button12Click
          end
          object Button13: TButton
            Left = 162
            Top = 64
            Width = 49
            Height = 32
            TabOrder = 4
            OnClick = Button13Click
          end
          object Button14: TButton
            Left = 217
            Top = 26
            Width = 49
            Height = 32
            TabOrder = 5
            OnClick = Button14Click
          end
          object Button15: TButton
            Left = 162
            Top = 26
            Width = 49
            Height = 32
            TabOrder = 6
            OnClick = Button15Click
          end
          object Button16: TButton
            Left = 217
            Top = 64
            Width = 49
            Height = 32
            TabOrder = 7
            OnClick = Button16Click
          end
          object Button17: TButton
            Left = 382
            Top = 102
            Width = 49
            Height = 32
            TabOrder = 8
            OnClick = Button17Click
          end
          object Button18: TButton
            Left = 327
            Top = 102
            Width = 49
            Height = 32
            TabOrder = 9
            OnClick = Button18Click
          end
          object Button19: TButton
            Left = 327
            Top = 140
            Width = 49
            Height = 32
            TabOrder = 10
            OnClick = Button19Click
          end
          object Button20: TButton
            Left = 272
            Top = 140
            Width = 49
            Height = 32
            TabOrder = 11
            OnClick = Button20Click
          end
          object Button31: TButton
            Left = 382
            Top = 178
            Width = 49
            Height = 32
            TabOrder = 12
            OnClick = Button31Click
          end
          object Button30: TButton
            Left = 382
            Top = 140
            Width = 49
            Height = 32
            TabOrder = 13
            OnClick = Button30Click
          end
          object Button29: TButton
            Left = 382
            Top = 64
            Width = 49
            Height = 32
            TabOrder = 14
            OnClick = Button29Click
          end
          object Button28: TButton
            Left = 327
            Top = 178
            Width = 49
            Height = 32
            TabOrder = 15
            OnClick = Button28Click
          end
          object Button27: TButton
            Left = 162
            Top = 178
            Width = 49
            Height = 32
            TabOrder = 16
            OnClick = Button27Click
          end
          object Button25: TButton
            Left = 217
            Top = 140
            Width = 49
            Height = 32
            TabOrder = 17
            OnClick = Button25Click
          end
          object Button26: TButton
            Left = 162
            Top = 140
            Width = 49
            Height = 32
            TabOrder = 18
            OnClick = Button26Click
          end
          object Button23: TButton
            Left = 217
            Top = 178
            Width = 49
            Height = 32
            TabOrder = 19
            OnClick = Button23Click
          end
          object Button24: TButton
            Left = 382
            Top = 26
            Width = 49
            Height = 32
            TabOrder = 20
            OnClick = Button24Click
          end
          object Button22: TButton
            Left = 162
            Top = 102
            Width = 49
            Height = 32
            TabOrder = 21
            OnClick = Button22Click
          end
          object Button7: TButton
            Left = 272
            Top = 178
            Width = 49
            Height = 32
            TabOrder = 22
            OnClick = Button7Click
          end
          object Button21: TButton
            Left = 217
            Top = 102
            Width = 49
            Height = 32
            TabOrder = 23
            OnClick = Button21Click
          end
          object CheckBox4: TCheckBox
            Left = 20
            Top = 34
            Width = 97
            Height = 17
            Caption = 'Enable'
            TabOrder = 24
            OnClick = CheckBox4Click
          end
          object Edit5: TEdit
            Left = 20
            Top = 106
            Width = 121
            Height = 24
            NumbersOnly = True
            TabOrder = 25
            TextHint = 'Item id'
            OnChange = Edit5Change
          end
          object CheckBox6: TCheckBox
            Left = 20
            Top = 80
            Width = 97
            Height = 17
            Caption = 'Place Blocks'
            TabOrder = 26
            OnClick = CheckBox6Click
          end
          object CheckBox5: TCheckBox
            Left = 20
            Top = 57
            Width = 97
            Height = 17
            Caption = 'Break Blocks'
            TabOrder = 27
            OnClick = CheckBox5Click
          end
          object CheckBox10: TCheckBox
            Left = 20
            Top = 140
            Width = 97
            Height = 17
            Caption = 'Get Remote'
            TabOrder = 28
            OnClick = CheckBox10Click
          end
          object CheckBox12: TCheckBox
            Left = 20
            Top = 163
            Width = 97
            Height = 17
            Caption = 'Auto Surg'
            Enabled = False
            TabOrder = 29
            OnClick = CheckBox10Click
          end
        end
        object TabSheet5: TTabSheet
          Caption = 'Spammer'
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Label13: TLabel
            Left = 3
            Top = 3
            Width = 30
            Height = 16
            Caption = 'Text:'
          end
          object Label14: TLabel
            Left = 3
            Top = 58
            Width = 48
            Height = 16
            Caption = 'Interval:'
          end
          object Label17: TLabel
            Left = 2
            Top = 139
            Width = 49
            Height = 16
            Caption = 'SB Text:'
            Visible = False
          end
          object Edit10: TEdit
            Left = 3
            Top = 25
            Width = 406
            Height = 24
            TabOrder = 0
            TextHint = 'Spam Text'
            OnChange = Edit10Change
          end
          object Edit11: TEdit
            Left = 57
            Top = 55
            Width = 121
            Height = 24
            NumbersOnly = True
            TabOrder = 1
            Text = '4000'
            TextHint = 'Interval'
          end
          object CheckBox9: TCheckBox
            Left = 3
            Top = 108
            Width = 153
            Height = 17
            Caption = 'Enable Auto Message'
            TabOrder = 2
          end
          object CheckBox8: TCheckBox
            Left = 3
            Top = 85
            Width = 153
            Height = 17
            Caption = 'Enable Chat Spammer'
            TabOrder = 3
            OnClick = CheckBox8Click
          end
          object Edit13: TEdit
            Left = 2
            Top = 161
            Width = 406
            Height = 24
            TabOrder = 4
            TextHint = 'SB Text'
            Visible = False
            OnChange = Edit13Change
          end
          object Button36: TButton
            Left = 2
            Top = 191
            Width = 89
            Height = 25
            Caption = 'Send SB'
            TabOrder = 5
            Visible = False
            OnClick = Button36Click
          end
          object Button37: TButton
            Left = 97
            Top = 191
            Width = 89
            Height = 25
            Caption = 'Take SB'
            TabOrder = 6
            Visible = False
            OnClick = Button37Click
          end
        end
        object TabSheet6: TTabSheet
          Caption = 'Misc'
          ImageIndex = 3
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Label15: TLabel
            Left = 3
            Top = 5
            Width = 30
            Height = 16
            Caption = 'Text:'
          end
          object Button34: TButton
            Left = 98
            Top = 138
            Width = 89
            Height = 25
            Caption = 'Send Packet'
            TabOrder = 0
            OnClick = Button34Click
          end
          object Edit12: TEdit
            Left = 2
            Top = 138
            Width = 42
            Height = 24
            NumbersOnly = True
            TabOrder = 1
            Text = '3'
          end
          object Memo1: TMemo
            Left = 2
            Top = 27
            Width = 185
            Height = 105
            TabOrder = 2
          end
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Item DB'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label2: TLabel
        Left = 3
        Top = 19
        Width = 54
        Height = 16
        Caption = 'Items.dat'
      end
      object Label3: TLabel
        Left = 303
        Top = 78
        Width = 68
        Height = 25
        Caption = 'Name: '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 303
        Top = 109
        Width = 80
        Height = 25
        Caption = 'Item ID:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Edit6: TEdit
        Left = 63
        Top = 16
        Width = 465
        Height = 24
        TabOrder = 0
      end
      object Button32: TButton
        Left = 534
        Top = 15
        Width = 75
        Height = 25
        Caption = 'Browse'
        TabOrder = 1
        OnClick = Button32Click
      end
      object SearchBox1: TSearchBox
        Left = 3
        Top = 46
        Width = 606
        Height = 24
        TabOrder = 2
        OnInvokeSearch = SearchBox1InvokeSearch
      end
      object ListView1: TListView
        Left = 3
        Top = 76
        Width = 609
        Height = 269
        Columns = <
          item
            AutoSize = True
            Caption = 'Name'
          end
          item
            AutoSize = True
            Caption = 'ID'
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        MultiSelect = True
        RowSelect = True
        ParentFont = False
        TabOrder = 3
        ViewStyle = vsReport
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Settings'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label5: TLabel
        Left = 19
        Top = 11
        Width = 68
        Height = 16
        Caption = 'Gt Version: '
      end
      object Label6: TLabel
        Left = 30
        Top = 40
        Width = 57
        Height = 16
        Caption = 'Server ip:'
      end
      object Label7: TLabel
        Left = 17
        Top = 74
        Width = 70
        Height = 16
        Caption = 'Server port:'
      end
      object Edit7: TEdit
        Left = 93
        Top = 11
        Width = 121
        Height = 24
        TabOrder = 0
        Text = '3.82'
        OnChange = Edit7Change
      end
      object Edit8: TEdit
        Left = 93
        Top = 41
        Width = 121
        Height = 24
        TabOrder = 1
        Text = '213.179.209.168'
        OnChange = Edit8Change
      end
      object Edit9: TEdit
        Left = 93
        Top = 71
        Width = 121
        Height = 24
        NumbersOnly = True
        TabOrder = 2
        Text = '17249'
        OnChange = Edit9Change
      end
      object Button35: TButton
        Left = 139
        Top = 101
        Width = 75
        Height = 25
        Caption = 'Pull Data'
        TabOrder = 3
        OnClick = Button35Click
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 408
    Top = 8
    object Save1: TMenuItem
      Caption = 'Save'
      OnClick = Save1Click
    end
    object Load1: TMenuItem
      Caption = 'Load'
      OnClick = Load1Click
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 452
    Top = 3
  end
  object SaveDialog1: TSaveDialog
    Left = 480
  end
  object client: TIdTCPClient
    ConnectTimeout = 0
    IPVersion = Id_IPv4
    Port = 0
    ReadTimeout = -1
    Left = 516
    Top = 3
  end
  object MainMenu1: TMainMenu
    Left = 556
    Top = 11
    object Styles1: TMenuItem
      Caption = 'Styles'
      object Glow1: TMenuItem
        Caption = 'Amakrits'
        OnClick = Style
      end
      object Windows10SlateGray1: TMenuItem
        Caption = 'Amethyst Kamri'
        OnClick = Style
      end
      object Carbon1: TMenuItem
        Caption = 'Aqua Graphite'
        OnClick = Style
      end
      object CharcoalDarkSlate1: TMenuItem
        Caption = 'Aqua Light Slate'
        OnClick = Style
      end
      object Auric1: TMenuItem
        Caption = 'Auric'
        OnClick = Style
      end
      object Carbon2: TMenuItem
        Caption = 'Carbon'
        OnClick = Style
      end
      object CharcoalDarkSlate2: TMenuItem
        Caption = 'Charcoal Dark Slate'
        OnClick = Style
      end
      object CobaltXEMedia1: TMenuItem
        Caption = 'Cobalt XEMedia'
        OnClick = Style
      end
      object CyanDusk1: TMenuItem
        Caption = 'Cyan Dusk'
        OnClick = Style
      end
      object CyanNight1: TMenuItem
        Caption = 'Cyan Night'
        OnClick = Style
      end
      object EmeraldLightSlate1: TMenuItem
        Caption = 'Emerald Light Slate'
        OnClick = Style
      end
      object Glossy1: TMenuItem
        Caption = 'Glossy'
        OnClick = Style
      end
      object Glow2: TMenuItem
        Caption = 'Glow'
        OnClick = Style
      end
      object GoldenGraphite1: TMenuItem
        Caption = 'Golden Graphite'
        OnClick = Style
      end
      object IcebergClassico1: TMenuItem
        Caption = 'Iceberg Classico'
        OnClick = Style
      end
      object Light1: TMenuItem
        Caption = 'Light'
        OnClick = Style
      end
      object Luna1: TMenuItem
        Caption = 'Luna'
        OnClick = Style
      end
      object MetropolisUI1: TMenuItem
        Caption = 'Metropolis UI'
        object MetropolisUIBlack1: TMenuItem
          Caption = 'Metropolis UI Black'
          OnClick = Style
        end
        object MetropolisUIBlue1: TMenuItem
          Caption = 'Metropolis UI Blue'
          OnClick = Style
        end
        object MetropolisUIGreen1: TMenuItem
          Caption = 'Metropolis UI Dark'
          OnClick = Style
        end
        object MetropolisUIGreen2: TMenuItem
          Caption = 'Metropolis UI Green'
          OnClick = Style
        end
      end
      object Obsidian1: TMenuItem
        Caption = 'Obsidian'
        OnClick = Style
      end
      object OnyxBlue1: TMenuItem
        Caption = 'Onyx Blue'
        OnClick = Style
      end
      object SapphireKamri1: TMenuItem
        Caption = 'Sapphire Kamri'
        OnClick = Style
      end
      object RubyGraphite1: TMenuItem
        Caption = 'Ruby Graphite'
        OnClick = Style
      end
      object Silver1: TMenuItem
        Caption = 'Silver'
        OnClick = Style
      end
      object Sky1: TMenuItem
        Caption = 'Sky'
        OnClick = Style
      end
      object SlateClassicco1: TMenuItem
        Caption = 'Slate Classico'
        OnClick = Style
      end
      object SmokeyQuartzKamri1: TMenuItem
        Caption = 'Smokey Quartz Kamri'
        OnClick = Style
      end
      object abletLight1: TMenuItem
        Caption = 'Tablet Light'
        OnClick = Style
      end
      object abletDark1: TMenuItem
        Caption = 'TabletDark'
        OnClick = Style
      end
      object urquosieGray1: TMenuItem
        Caption = 'Turquoise Gray'
        OnClick = Style
      end
      object Windows101: TMenuItem
        Caption = 'Windows10'
        object Windows10Blue1: TMenuItem
          Caption = 'Windows10 Blue'
          OnClick = Style
        end
        object Windows10Dark1: TMenuItem
          Caption = 'Windows10 Dark'
          OnClick = Style
        end
        object Windows10Green1: TMenuItem
          Caption = 'Windows10 Green'
          OnClick = Style
        end
        object Windows10Purple1: TMenuItem
          Caption = 'Windows10 Purple'
          OnClick = Style
        end
        object Windows10SlateGray2: TMenuItem
          Caption = 'Windows10 SlateGray'
          OnClick = Style
        end
      end
    end
    object About1: TMenuItem
      Caption = 'About'
      object Social1: TMenuItem
        Caption = 'Social'
        object Youtube1: TMenuItem
          Caption = 'Youtube'
          OnClick = Youtube1Click
        end
        object Discord1: TMenuItem
          Caption = 'Discord'
          OnClick = Discord1Click
        end
      end
      object Update1: TMenuItem
        Caption = 'Update'
        OnClick = Update1Click
      end
    end
  end
  object Timer1: TTimer
    Interval = 6000
    OnTimer = Timer1Timer
    Left = 492
    Top = 275
  end
  object RESTRequest1: TRESTRequest
    Client = RESTClient1
    Method = rmPOST
    Params = <
      item
        Name = 'version'
        Value = '3'
      end
      item
        Name = 'platform'
        Value = '0'
      end
      item
        Kind = pkHTTPHEADER
        Name = 'protocol'
        Value = '147'
      end>
    Response = RESTResponse1
    SynchronizedEvents = False
    Left = 588
    Top = 65523
  end
  object RESTClient1: TRESTClient
    AutoCreateParams = False
    BaseURL = 'http://growtopia1.com/growtopia/server_data.php'
    Params = <>
    Left = 516
    Top = 65523
  end
  object RESTResponse1: TRESTResponse
    Left = 548
    Top = 65523
  end
  object MainMenu2: TMainMenu
    Left = 593
    Top = 286
  end
end
