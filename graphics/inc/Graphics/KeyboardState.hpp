#pragma once

#include "Config.hpp"
#include "KeyCodes.hpp"

namespace Graphics
{
/// <summary>
/// A class that represents the current state of the keyboard.
/// </summary>
struct SR_API KeyboardState
{
    // clang-format off
    bool Reserved0 : 1;           // Unused
    bool LeftButton : 1;          // Left mouse button, 0x01
    bool RightButton : 1;         // Right mouse button, 0x02
    bool Cancel : 1;              // Cancel key, 0x03
    bool MButton : 1;             // Middle mouse button, 0x04
    bool XButton1 : 1;            // X1 mouse button, 0x05
    bool XButton2 : 1;            // X2 mouse button, 0x06
    bool Reserved1 : 1;           // 0x07 is undefined.
    // ============ ( 1 byte ) ============================
    bool Back : 1;                // The back button, 0x08
    bool Tab : 1;                 // The Tab key, 0x09
    bool Reserved2 : 2;           // 0x0A-0B are undefined
    bool Clear : 1;               // The Clear key, 0x0C
    bool Enter : 1;               // The Enter key, 0x0D
    bool Reserved3 : 2;           // 0x0E-0F are undefined
    // ============ ( 2 bytes ) ===========================
    bool ShiftKey : 1;            // The Shift key, 0x10
    bool ControlKey : 1;          // The Ctrl key, 0x11
    bool AltKey : 1;              // The Alt key, 0x12
    bool Pause : 1;               // The Pause key, 0x13
    bool CapsLock : 1;            // The Caps Lock key, 0x14
    bool KanaMode : 1;            // IMI Kana mode, 0x15
    bool ImeOn : 1;               // IME On, 0x16
    bool JunjaMode : 1;           // IME Janja mode, 0x17
    // ============ ( 3 bytes ) ===========================
    bool FinalMode : 1;           // IME Final mode, 0x18
    bool KanjiMode : 1;           // IME Kanji mode, 0x19
    bool ImeOff : 1;              // IME Off, 0x1A
    bool Escape : 1;              // The Escape key, 0x1B
    bool IMEConvert : 1;          // IME convert key, 0x1C
    bool IMENoConvert : 1;        // IME no convert key, 0x1D
    bool IMEAccept : 1;           // IME accept key, 0x1E
    bool IMEModeChange : 1;       // IME mode change key, 0x1F
    // ============ ( 4 bytes ) ===========================
    bool Space : 1;               // The Space key, 0x20
    bool PageUp : 1;              // The Page Up key, 0x21
    bool PageDown : 1;            // The Page Down key, 0x22
    bool End : 1;                 // The End key, 0x23
    bool Home : 1;                // The Home key, 0x24
    bool Left : 1;                // The Left arrow key, 0x25
    bool Up : 1;                  // The Up arrow key, 0x26
    bool Right : 1;               // The Right arrow key, 0x27
    // ============ ( 5 bytes ) ===========================
    bool Down : 1;                // The Down arrow key, 0x28
    bool Select : 1;              // The Select key, 0x29
    bool Print : 1;               // The Print key, 0x2A
    bool Execute : 1;             // The Execute key, 0x2B
    bool PrintScreen : 1;         // The Print screen key, 0x2C
    bool Insert : 1;              // The Insert key, 0x2D
    bool Delete : 1;              // The Delete key, 0x2E
    bool Help : 1;                // The Help key, 0x2F
    // ============ ( 6 bytes ) ===========================
    bool D0 : 1;                  // 0, 0x30
    bool D1 : 1;                  // 1, 0x31
    bool D2 : 1;                  // 2, 0x32
    bool D3 : 1;                  // 3, 0x33
    bool D4 : 1;                  // 4, 0x34
    bool D5 : 1;                  // 5, 0x35
    bool D6 : 1;                  // 6, 0x36
    bool D7 : 1;                  // 7, 0x37
    // ============ ( 7 bytes ) ===========================
    bool D8 : 1;                  // 8, 0x38
    bool D9 : 1;                  // 9, 0x39
    bool Reserved4 : 6;           // 0x3A - 3F are undefined.
    // ============ ( 8 bytes ) ===========================
    bool Reserved5 : 1;           // 0x40 is undefined.
    bool A : 1;                   // A, 0x41
    bool B : 1;                   // B, 0x42
    bool C : 1;                   // C, 0x43
    bool D : 1;                   // D, 0x44
    bool E : 1;                   // E, 0x45
    bool F : 1;                   // F, 0x46
    bool G : 1;                   // G, 0x47
    // ============ ( 9 bytes ) ===========================
    bool H : 1;                   // H, 0x48
    bool I : 1;                   // I, 0x49
    bool J : 1;                   // J, 0x4A
    bool K : 1;                   // K, 0x4B
    bool L : 1;                   // L, 0x4C
    bool M : 1;                   // M, 0x4D
    bool N : 1;                   // N, 0x4E
    bool O : 1;                   // O, 0x4F
    // ============ ( 10 bytes ) ===========================
    bool P : 1;                   // P, 0x50
    bool Q : 1;                   // Q, 0x51
    bool R : 1;                   // R, 0x52
    bool S : 1;                   // S, 0x53
    bool T : 1;                   // T, 0x54
    bool U : 1;                   // U, 0x55
    bool V : 1;                   // V, 0x56
    bool W : 1;                   // W, 0x57
    // ============ ( 11 bytes ) ===========================
    bool X : 1;                   // X, 0x58
    bool Y : 1;                   // Y, 0x59
    bool Z : 1;                   // Z, 0x5A
    bool LeftWindows : 1;         // Left Windows key, 0x5B
    bool RightWindows : 1;        // Right Windows key, 0x5C
    bool Apps : 1;                // Apps key, 0x5D
    bool Reserved6 : 1;           // 0x5E is reserved
    bool Sleep : 1;               // The Sleep key, 0x5F
    // ============ ( 12 bytes ) ===========================
    bool NumPad0 : 1;             // The Numeric keybad 0 key, 0x60
    bool NumPad1 : 1;             // The Numeric keybad 1 key, 0x61
    bool NumPad2 : 1;             // The Numeric keybad 2 key, 0x62
    bool NumPad3 : 1;             // The Numeric keybad 3 key, 0x63
    bool NumPad4 : 1;             // The Numeric keybad 4 key, 0x64
    bool NumPad5 : 1;             // The Numeric keybad 5 key, 0x65
    bool NumPad6 : 1;             // The Numeric keybad 6 key, 0x66
    bool NumPad7 : 1;             // The Numeric keybad 7 key, 0x67
    // ============ ( 13 bytes ) ===========================
    bool NumPad8 : 1;             // The Numeric keybad 8 key, 0x68
    bool NumPad9 : 1;             // The Numeric keybad 9 key, 0x69
    bool Multiply : 1;            // The Multiply key, 0x6A
    bool Add : 1;                 // The Add key, 0x6B
    bool Separator : 1;           // The Separator key, 0x6C
    bool Subtract : 1;            // The Subtract key, 0x6D
    bool Decimal : 1;             // The Decimal key, 0x6E
    bool Divide : 1;              // The Divide key, 0x6F
    // ============ ( 14 bytes ) ===========================
    bool F1 : 1;                  // The F1 key, 0x70
    bool F2 : 1;                  // The F2 key, 0x71
    bool F3 : 1;                  // The F3 key, 0x72
    bool F4 : 1;                  // The F4 key, 0x73
    bool F5 : 1;                  // The F5 key, 0x74
    bool F6 : 1;                  // The F6 key, 0x75
    bool F7 : 1;                  // The F7 key, 0x76
    bool F8 : 1;                  // The F8 key, 0x77
    // ============ ( 15 bytes ) ===========================
    bool F9 : 1;                  // The F9 key, 0x78
    bool F10 : 1;                 // The F10 key, 0x79
    bool F11 : 1;                 // The F11 key, 0x7A
    bool F12 : 1;                 // The F12 key, 0x7B
    bool F13 : 1;                 // The F13 key, 0x7C
    bool F14 : 1;                 // The F14 key, 0x7D
    bool F15 : 1;                 // The F15 key, 0x7E
    bool F16 : 1;                 // The F16 key, 0x7F
    // ============ ( 16 bytes ) ===========================
    bool F17 : 1;                 // The F17 key, 0x80
    bool F18 : 1;                 // The F18 key, 0x81
    bool F19 : 1;                 // The F19 key, 0x82
    bool F20 : 1;                 // The F20 key, 0x83
    bool F21 : 1;                 // The F21 key, 0x84
    bool F22 : 1;                 // The F22 key, 0x85
    bool F23 : 1;                 // The F23 key, 0x86
    bool F24 : 1;                 // The F24 key, 0x87
    // ============ ( 17 bytes ) ===========================
    bool Reserved7 : 8;           // 0x88 - 8F are unassigend
    // ============ ( 18 bytes ) ===========================
    bool NumLock : 1;             // The Num Lock key, 0x90
    bool Scroll : 1;              // The Scroll Lock key, 0x91
    bool Reserved9 : 6;           // 0x92 - 0x97 are OEM specific
    // ============ ( 19 bytes ) ===========================
    bool Reserved10 : 8;          // 0x98 - 9F are unassigned
    // ============ ( 20 bytes ) ===========================
    bool LeftShift : 1;           // The Left Shift key, 0xA0
    bool RightShift : 1;          // The Right Shift key, 0xA1
    bool LeftControl : 1;         // The Left Control key, 0xA2
    bool RightControl : 1;        // The Right Control key, 0xA3
    bool LeftAlt : 1;             // The Left Alt key, 0xA4
    bool RightAlt : 1;            // The Right Alt key, 0xA5
    bool BrowserBack : 1;         // The Browser Back key, 0xA6
    bool BrowserForward : 1;      // The Browser Forward key, 0xA7
    // ============ ( 21 bytes ) ===========================
    bool BrowserRefresh : 1;      // The Browser Refresh key, 0xA8
    bool BrowserStop : 1;         // The Browser Stop key, 0xA9
    bool BrowserSearch : 1;       // The Browser Search key, 0xAA
    bool BrowserFavorites : 1;    // The Browser Favorites key, 0xAB
    bool BrowserHome : 1;         // The Browser Home key, 0xAC
    bool VolumeMute : 1;          // The Volume Mute key, 0xAD
    bool VolumeDown : 1;          // The Volume Down key, 0xAE
    bool VolumeUp : 1;            // The Volume Up key, 0xAF
    // ============ ( 22 bytes ) ===========================
    bool MediaNextTrack : 1;      // The Next Track key, 0xB0
    bool MediaPreviousTrack : 1;  // The Previous Track key, 0xB1
    bool MediaStop : 1;           // The Stop Media key, 0xB2
    bool MediaPlayPause : 1;      // The Play/Pause Media key, 0xB3
    bool LaunchMail : 1;          // The Start Mail key, 0xB4
    bool SelectMedia : 1;         // The Select Media key, 0xB5
    bool LaunchApplication1 : 1;  // Start Application 1 key, 0xB6
    bool LaunchApplication2 : 1;  // Start Application 2 key, 0xB7
    // ============ ( 23 bytes ) ===========================
    bool Reserved12 : 2;          // 0xB8 - B9 are reserved.
    bool OemSemicolon : 1;        // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key, 0xBA
    bool OemPlus : 1;             // For any country/region, the '+' key, 0xBB
    bool OemComma : 1;            // For any country/region, the ',' key, 0xBC
    bool OemMinus : 1;            // For any country/region, the '-' key, 0xBD
    bool OemPeriod : 1;           // For any country/region, the '.' key, 0xBE
    bool OemQuestion : 1;         // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key, 0xBF
    // ============ ( 24 bytes ) ===========================
    bool OemTilde : 1;            // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key, 0xC0
    bool Reserved13 : 7;          // 0xC1 - C7 are reserved.
    // ============ ( 25 bytes ) ===========================
    bool Reserved15 : 8;          // 0xC9 - CF are reserved.
    // ============ ( 26 bytes ) ===========================
    bool Reserved16 : 8;          // 0xD0 - D7 are reserved.
    // ============ ( 27 bytes ) ===========================
    bool Reserved17 : 3;          // 0xD8 - DA are unassigned.
    bool OemOpenBrackets : 1;     // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key, 0xDB
    bool OemPipe : 1;             // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key, 0xDC
    bool OemCloseBrackets : 1;    // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key, 0xDD
    bool OemQuotes : 1;           // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key, 0xDE
    bool Oem8 : 1;                // Used for miscellaneous characters; it can vary by keyboard, 0xDF
    // ============ ( 28 bytes ) ===========================
    bool Reserved18 : 2;          // 0xE0 - E1 are reserved
    bool OemBackslash : 1;        // Either the angle bracket key or the backslash key on the RT 102-key keyboard, 0xE2
    bool Reserved19 : 2;          // 0xE3 - E4 are OEM specific
    bool Process : 1;             // IME Process key, 0xE5
    bool Reserved20 : 1;          // 0xE6 is OEM specific
    bool Packet : 1;              // Used to pass Unicode characters as if they were keystrokes. The Packet key value is the low word of a 32-bit virtual-key value used for non-keyboard input methods, 0xE7
    // ============ ( 29 bytes ) ===========================
    bool Reserved21 : 1;          // 0xE8 unassigned.
    bool Reserved22 : 7;          // 0xE9 - EF unassigned.
    // ============ ( 30 bytes ) ===========================
    bool Reserved23 : 6;          // 0xF0 - F5 is OEM specific.
    bool Attn : 1;                // The Attn key, 0xF6
    bool CrSel : 1;               // The CrSel key, 0xF7
    // ============ ( 31 bytes ) ===========================
    bool ExSel : 1;               // The ExSel key, 0xF8
    bool EraseEof : 1;            // The Erase EOF key, 0xF9
    bool Play : 1;                // The Play key, 0xFA
    bool Zoom : 1;                // The Zoom key, 0xFB
    bool NoName : 1;              // Reserved, 0xFC
    bool Pa1 : 1;                 // The PA1 key, 0xFD
    bool OemClear : 1;            // The Clear key, 0xFE
    bool Reserved25: 1;           // 0xFF is reserved.
    // ============ ( 32 bytes ) ===========================
    // clang-format on

    /// <summary>
    /// Check if a key is being pressed on the keyboard.
    /// </summary>
    /// <param name="key">The key to query.</param>
    /// <returns>`true` if the key is pressed on the keyboard, `false` otherwise.</returns>
    bool isKeyDown( KeyCode key ) const noexcept;

    /// <summary>
    /// Check if a key is not being pressed on the keyboard.
    /// </summary>
    /// <param name="key">The key to query.</param>
    /// <returns>`true` if the key is not pressed, `false` otherwise.</returns>
    bool isKeyUp( KeyCode key ) const noexcept;

    bool operator==( const KeyboardState& ) const = default;
    bool operator!=( const KeyboardState& ) const = default;
};
}  // namespace Graphics