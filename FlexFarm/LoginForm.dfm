object Form2: TForm2
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Login'
  ClientHeight = 111
  ClientWidth = 306
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 19
  object Edit1: TEdit
    Left = 8
    Top = 8
    Width = 289
    Height = 27
    TabOrder = 0
    TextHint = 'Username'
  end
  object Edit2: TEdit
    Left = 8
    Top = 41
    Width = 289
    Height = 27
    PasswordChar = '*'
    TabOrder = 1
    TextHint = 'Password'
  end
  object Button1: TButton
    Left = 224
    Top = 74
    Width = 73
    Height = 25
    Caption = 'Login'
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 96
    Top = 74
    Width = 122
    Height = 25
    Caption = 'Free Version'
    ModalResult = 11
    TabOrder = 3
  end
end
