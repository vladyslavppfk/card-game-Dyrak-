#pragma once

#include "SettingsForm.h"

namespace Dyrakgame {
    ref class GameWindow;
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Gameform : public System::Windows::Forms::Form {
    public:
        // Объявляем и сразу инициализируем внутри тела ref-класса:
        static bool FullscreenModeStatic = false;

        Gameform(void) {
            InitializeComponent();
            // здесь можно, при необходимости, также перезаписать FullscreenModeStatic,
            // но обычно достаточно того, что он уже = false
        }

    protected:
        ~Gameform() {
            if (components) {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;

        Label^ label1;
        Button^ buttonstart;
        Button^ buttonexit;
        Button^ buttonsettings;
        bool fullscreenMode;

    public:
        property bool FullscreenMode {
            bool get() { return fullscreenMode; }
            void set(bool value) { fullscreenMode = value; }
        }

        void ApplyFullscreen() {
            if (FullscreenModeStatic) {
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
                this->WindowState = FormWindowState::Maximized;
            }
            else {
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
                this->WindowState = FormWindowState::Normal;
                this->ClientSize = System::Drawing::Size(1000, 690);
            }
        }

#pragma region Windows Form Designer generated code
        void InitializeComponent() {
            this->SuspendLayout();
            // 
            // Gameform
            // 
            this->StartPosition = FormStartPosition::CenterScreen;
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1000, 690);
            this->Text = L"Gameform";
            this->BackColor = System::Drawing::Color::ForestGreen;
            this->ResumeLayout(false);

            // label1
            label1 = gcnew Label();
            label1->Text = L"ДУРАК";
            label1->Font = gcnew System::Drawing::Font("Arial", 20, FontStyle::Bold);
            label1->Size = System::Drawing::Size(200, 40);
            label1->Location = System::Drawing::Point(400, 100);
            label1->ForeColor = System::Drawing::Color::White;
            label1->TextAlign = ContentAlignment::MiddleCenter;
            this->Controls->Add(label1);
            label1->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);

            // buttonstart
            buttonstart = gcnew Button();
            buttonstart->Text = "Почати гру";
            buttonstart->Size = System::Drawing::Size(200, 50);
            buttonstart->Location = System::Drawing::Point(400, 250);
            buttonstart->Click += gcnew System::EventHandler(this, &Gameform::buttonstart_Click);
            this->Controls->Add(buttonstart);
            buttonstart->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);

            // buttonsettings
            buttonsettings = gcnew Button();
            buttonsettings->Text = "Налаштування";
            buttonsettings->Size = System::Drawing::Size(200, 50);
            buttonsettings->Location = System::Drawing::Point(400, 350);
            buttonsettings->Click += gcnew System::EventHandler(this, &Gameform::buttonsettings_Click);
            this->Controls->Add(buttonsettings);
            buttonsettings->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right);

            // buttonexit
            buttonexit = gcnew Button();
            buttonexit->Text = "Вийти";
            buttonexit->Size = System::Drawing::Size(200, 50);
            buttonexit->Location = System::Drawing::Point(400, 450);
            buttonexit->Click += gcnew System::EventHandler(this, &Gameform::buttonexit_Click);
            this->Controls->Add(buttonexit);
            buttonexit->Anchor = static_cast<AnchorStyles>(AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right);
        }
#pragma endregion

        // Обработчики объявляем здесь, реализуем чуть ниже
        void buttonstart_Click(Object^ sender, EventArgs^ e);
        void GameWindowClosed(Object^ sender, FormClosedEventArgs^ e);
        void buttonexit_Click(Object^ sender, EventArgs^ e);
        void buttonsettings_Click(Object^ sender, EventArgs^ e);
    };
} // namespace Dyrakgame
