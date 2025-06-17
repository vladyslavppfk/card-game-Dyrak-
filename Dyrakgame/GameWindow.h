#pragma once
#include <vector>
#include "SettingsForm.h"
#include "EndGameForm.h"
#include "Gameform.h"

namespace Dyrakgame {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Collections::Generic;

    public enum class Suit {
        Hearts,
        Diamonds,
        Clubs,
        Spades
    };
    public enum class Rank {
        Six = 6,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace
    };

    ref class Card;
    ref class Deck;
    ref class AnimatedCard;
    ref class GameWindow;
    ref class SettingsForm;

    public ref class Card {
    public:
        Point originalPosition;
        Suit suit;
        Rank rank;
        PictureBox^ pictureBox;

        Card(Suit s, Rank r, Image^ img) {
            originalPosition = Point(0, 0);
            suit = s;
            rank = r;
            String^ fileName = r.ToString() + "_" + s.ToString() + ".png";
            String^ filepath = "C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\" + fileName;

            pictureBox = gcnew PictureBox();
            pictureBox->SizeMode = PictureBoxSizeMode::StretchImage;
            pictureBox->Size = System::Drawing::Size(80, 120);
            pictureBox->Tag = filepath;
            try {
                pictureBox->Image = Image::FromFile(filepath);
            }
            catch (...) {
                MessageBox::Show("Error: " + filepath);
            }
        }
    };

    public ref class Deck {
    private:
        List<Card^>^ cards;
    public:
        Deck() {
            cards = gcnew List<Card^>();
            array<Suit>^ suits = { Suit::Clubs, Suit::Diamonds, Suit::Hearts, Suit::Spades };
            array<Rank>^ ranks = { Rank::Six, Rank::Seven, Rank::Eight, Rank::Nine, Rank::Ten, Rank::Jack, Rank::Queen, Rank::King, Rank::Ace };
            Random^ rand = gcnew Random();
            for each(Suit s in suits) {
                for each(Rank r in ranks) {
                    cards->Add(gcnew Card(s, r, nullptr));
                }
            }
            Shuffle();
        }
        void Shuffle() {
            Random^ rand = gcnew Random();
            for (int i = 0; i < cards->Count; i++) {
                int j = rand->Next(cards->Count);
                Card^ temp = cards[i];
                cards[i] = cards[j];
                cards[j] = temp;
            }
        }
        Card^ DrawCard() {
            if (cards->Count == 0) return nullptr;
            Card^ c = cards[0];
            cards->RemoveAt(0);
            return c;
        }
        int RemainingCards() {
            return cards->Count;
        }
    };

    public ref class AnimatedCard {
    private:
        void MoveCard(Object^ sender, EventArgs^ e) {
            int step = 15;
            int dx = targetPosition.X - card->pictureBox->Left;
            int dy = targetPosition.Y - card->pictureBox->Top;
            if (Math::Abs(dx) <= step && Math::Abs(dy) <= step) {
                card->pictureBox->Location = targetPosition;
                timer->Stop();
            }
            else {
                card->pictureBox->Left += Math::Sign(dx) * Math::Min(step, Math::Abs(dx));
                card->pictureBox->Top += Math::Sign(dy) * Math::Min(step, Math::Abs(dy));
            }
        }

    public:
        Card^ card;
        Point startLocation;
        Point targetPosition;
        Timer^ timer;

        AnimatedCard(Card^ c, Point p, Point target) {
            card = c;
            startLocation = p;
            targetPosition = target;
            timer = gcnew Timer();
            timer->Interval = 10;
            timer->Tick += gcnew EventHandler(this, &AnimatedCard::MoveCard);
        }

        void StartAnimation() {
            card->pictureBox->Location = startLocation;
            timer->Start();
        }
    };

    public ref class GameWindow : public System::Windows::Forms::Form {
    private:
        Timer^ botMoveTimer;
        Card^ draggedCard;
        Point mouseOffset;
        Card^ trumpCardData;
        List<Tuple<Card^, Card^>^>^ tablePairs;
        List<Card^>^ throwInCards;
        List<Card^>^ tableCards;
        bool playerTurn;
        PictureBox^ deckPicture;
        Deck^ deck;
        List<Card^>^ playerHand;
        List<Card^>^ opponentHand;
        PictureBox^ trumpCard;
        Point deckPosition;
        Suit trumpSuit;
        Button^ endTurnButton;
        Button^ takeButton;
        bool isDefending = false;
        Card^ lastAttackCard = nullptr;
        Label^ opponentCardsLabel;
        Label^ deckCountLabel;
        bool trumpTaken = false;
        void UpdateButtonStates() {
            endTurnButton->Enabled = playerTurn && !isDefending;
            takeButton->Enabled = playerTurn && isDefending;
        }

    public:
        GameWindow() {
            this->StartPosition = FormStartPosition::CenterScreen;
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1000, 690);
            this->BackColor = System::Drawing::Color::DarkGreen;
            this->Text = "Gamefield";
            if (Gameform::FullscreenModeStatic) {
                this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
                this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
            }

            this->Resize += gcnew EventHandler(this, &GameWindow::GameWindow_Resize);
            this->Shown += gcnew EventHandler(this, &GameWindow::GameWindow_OnShown);


            this->Resize += gcnew EventHandler(this, &GameWindow::GameWindow_Resize);
            this->Shown += gcnew EventHandler(this, &GameWindow::GameWindow_OnShown);

            tableCards = gcnew List<Card^>();
            tablePairs = gcnew List<Tuple<Card^, Card^>^>();

            endTurnButton = gcnew Button();
            endTurnButton->Text = "Відбій";
            endTurnButton->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
            endTurnButton->Size = Drawing::Size(100, 40);
            endTurnButton->Click += gcnew EventHandler(this, &GameWindow::EndTurn_Click);
            this->Controls->Add(endTurnButton);

            takeButton = gcnew Button();
            takeButton->Text = "Взяти";
            takeButton->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
            takeButton->Size = Drawing::Size(100, 40);
            takeButton->Click += gcnew EventHandler(this, &GameWindow::Take_Click);
            this->Controls->Add(takeButton);

            opponentCardsLabel = gcnew Label();
            opponentCardsLabel->AutoSize = true;
            opponentCardsLabel->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Left);
            opponentCardsLabel->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
            opponentCardsLabel->ForeColor = System::Drawing::Color::White;
            opponentCardsLabel->BackColor = System::Drawing::Color::Transparent;
            opponentCardsLabel->TextAlign = ContentAlignment::MiddleLeft;
            this->Controls->Add(opponentCardsLabel);

            deckCountLabel = gcnew Label();
            deckCountLabel->AutoSize = true;
            deckCountLabel->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
            deckCountLabel->Font = gcnew System::Drawing::Font("Arial", 12, FontStyle::Bold);
            deckCountLabel->ForeColor = System::Drawing::Color::White;
            this->Controls->Add(deckCountLabel);

            deckPicture = gcnew PictureBox();
            deckPicture->Size = System::Drawing::Size(80, 120);
            deckPicture->SizeMode = PictureBoxSizeMode::StretchImage;
            deckPicture->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
            this->Controls->Add(deckPicture);

            botMoveTimer = gcnew Timer();
            botMoveTimer->Interval = 1500;
            botMoveTimer->Tick += gcnew EventHandler(this, &GameWindow::BotMoveTimer_Tick);

            StartGame();
            UpdateButtonStates();
            GameWindow_Resize(nullptr, nullptr);
        }

    private:
        void CheckGameEnd() {
            int playerCount = playerHand->Count;
            int botCount = opponentHand->Count;

            String^ msg = nullptr;
            if (playerCount == 0 && botCount > 0) {
                msg = "Ви перемогли!!!";
            }
            else if (botCount == 0 && playerCount > 0) {
                msg = "Ви програли, ви ДУРАК!!!";
            }
            else if (playerCount == 0 && botCount == 0) {
                msg = "Нічия, ніхто не дурак!!!";
            }

            if (msg != nullptr) {
                EndGameForm^ endForm = gcnew EndGameForm(msg);
                auto result = endForm->ShowDialog(this);
                if (result == System::Windows::Forms::DialogResult::Retry)
                    this->DialogResult = System::Windows::Forms::DialogResult::Retry;
                else
                    this->DialogResult = System::Windows::Forms::DialogResult::Abort;

                this->Close();
            }
        }

        void GameWindow_OnShown(Object^ sender, EventArgs^ e) {
            GameWindow_Resize(nullptr, nullptr);
        }
        void GameWindow_Resize(Object^ sender, EventArgs^ e) {
            deckPosition = Point(this->ClientSize.Width - 200, this->ClientSize.Height / 2 - 60);
            deckPicture->Location = deckPosition;
            deckPicture->Image = Image::FromFile("C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\back.png");
            if (trumpCard != nullptr) {
                trumpCard->Location = Point(deckPosition.X - 40, deckPosition.Y + 18);
            }
            endTurnButton->Location = Point(this->ClientSize.Width - 120, this->ClientSize.Height - 90);
            takeButton->Location = Point(this->ClientSize.Width - 120, this->ClientSize.Height - 190);
            deckCountLabel->Location = Point(this->ClientSize.Width - 120, this->ClientSize.Height / 2 + 60);
            opponentCardsLabel->Location = Point(20, 20);
            SetOpponentHandPositions();
            SetPlayerHandPositions();

        }
        void SetOpponentHandPositions() {
            int total = opponentHand->Count;
            if (total == 0) return;

            int spacing = Math::Min(90, (this->ClientSize.Width - 100) / total);
            int x = 50;
            for (int i = 0; i < total; i++) {
                Card^ c = opponentHand[i];
                c->pictureBox->Image = Image::FromFile("C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\back.png");
                Point target = Point(x, 50);
                if (!this->Controls->Contains(c->pictureBox)) {
                    c->pictureBox->Location = deckPosition;
                    this->Controls->Add(c->pictureBox);
                }
                c->pictureBox->Location = target;
                x += spacing;
            }
            UpdateOpponentCardsLabel();
        }

        void SetPlayerHandPositions() {
            int cardCount = playerHand->Count;
            if (cardCount == 0) return;

            const int cardWidth = 80;
            const int step = 90;
            int totalWidth = step * (cardCount - 1) + cardWidth;
            int rightBoundary = takeButton->Location.X - 20;
            int maxWidth = rightBoundary - 50;
            int x;
            if (totalWidth < maxWidth) {
                x = (maxWidth - totalWidth) / 2 + 50;
            }
            else {
                x = (this->ClientSize.Width - totalWidth) / 2;
            }

            for (int i = 0; i < cardCount; i++) {
                Point target = Point(x, this->ClientSize.Height - 240);
                Card^ c = playerHand[i];
                c->originalPosition = target;

                if (!this->Controls->Contains(c->pictureBox)) {
                    c->pictureBox->Location = deckPosition;
                    this->Controls->Add(c->pictureBox);
                }
                c->pictureBox->Location = target;
                c->pictureBox->MouseDown -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                c->pictureBox->MouseMove -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                c->pictureBox->MouseUp -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);

                c->pictureBox->MouseDown += gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                c->pictureBox->MouseMove += gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                c->pictureBox->MouseUp += gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);

                x += step;
            }
        }
        void BotMoveTimer_Tick(Object^ sender, EventArgs^ e) {
            botMoveTimer->Stop();
            this->BotMove(opponentHand, tableCards, trumpSuit);
            UpdateOpponentCardsLabel();
            UpdateButtonStates();
        }

        void UpdateOpponentCardsLabel() {
            if (opponentHand->Count == 0) {
                opponentCardsLabel->Text = "Бот: 0 карт";
                return;
            }
            String^ text = "Бот: ";
            for (int i = 0; i < opponentHand->Count; i++) {
                Card^ c = opponentHand[i];
                String^ rankName = c->rank.ToString();
                String^ suitName = c->suit.ToString();
                text += String::Format("{0} of {1}", rankName, suitName);
                if (i < opponentHand->Count - 1)
                    text += ", ";
            }
            opponentCardsLabel->Text = text;
        }

        int CompareCards(Card^ a, Card^ b) {
            bool aIsTrump = a->suit == trumpSuit;
            bool bIsTrump = b->suit == trumpSuit;
            if (aIsTrump != bIsTrump)
                return aIsTrump ? 1 : -1;
            return (int)a->rank - (int)b->rank;
        }

        bool CanBeat(Card^ attackCard, Card^ defendCard, Suit trumpSuit) {
            if (defendCard->suit == attackCard->suit) {
                return (int)defendCard->rank > (int)attackCard->rank;
            }
            else if (defendCard->suit == trumpSuit && attackCard->suit != trumpSuit) {
                return true;
            }
            return false;
        }

        Card^ DrawFromDeck() {
            Card^ c = deck->DrawCard();
            if (c != nullptr) {
                return c;
            }
            if (!trumpTaken) {
                trumpTaken = true;
                String^ facePath = (String^)trumpCardData->pictureBox->Tag;
                Image^ face = Image::FromFile(facePath);
                trumpCardData->pictureBox->Image = face;
                trumpCardData->pictureBox->SizeMode = PictureBoxSizeMode::StretchImage;
                trumpCardData->pictureBox->Size = System::Drawing::Size(80, 120);
                return trumpCardData;
            }
            return nullptr;
        }

        void UpdateDeckCountLabel() {
            int remaining = deck->RemainingCards() + (trumpTaken ? 0 : 1);
            deckCountLabel->Text = String::Format("В колоді: {0}", remaining);
        }

        void RedrawOpponentHandIncremental(int oldCount) {
            int total = opponentHand->Count;
            if (total == 0) {
                UpdateOpponentCardsLabel();
                return;
            }
            int spacing = Math::Min(90, (this->ClientSize.Width - 100) / total);

            for (int i = 0; i < total; i++) {
                Card^ c = opponentHand[i];
                Point target = Point(50 + i * spacing, 50);

                if (i < oldCount) {
                    Point current = c->pictureBox->Location;
                    AnimatedCard^ anim = gcnew AnimatedCard(c, current, target);
                    this->Controls->Add(c->pictureBox);
                    c->pictureBox->BringToFront();
                    anim->StartAnimation();
                }
                else {
                    c->pictureBox->Image = Image::FromFile(
                        "C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\back.png");
                    c->pictureBox->Location = deckPosition;
                    if (!this->Controls->Contains(c->pictureBox))
                        this->Controls->Add(c->pictureBox);
                    AnimatedCard^ anim = gcnew AnimatedCard(c, deckPosition, target);
                    c->pictureBox->BringToFront();
                    anim->StartAnimation();
                }
            }
            UpdateOpponentCardsLabel();
        }

        void RefillBotHand() {
            int prevCount = opponentHand->Count;

            while (opponentHand->Count < 6) {
                Card^ card = DrawFromDeck();
                if (card == nullptr)
                    break;
                opponentHand->Add(card);
                card->pictureBox->Location = deckPosition;
                card->pictureBox->Image = Image::FromFile("C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\back.png");
                if (!this->Controls->Contains(card->pictureBox))
                    this->Controls->Add(card->pictureBox);
            }

            SetOpponentHandPositions();

            for (int i = prevCount; i < opponentHand->Count; i++) {
                Card^ c = opponentHand[i];
                Point finalPos = c->pictureBox->Location;
                c->pictureBox->Location = deckPosition;
                AnimatedCard^ anim = gcnew AnimatedCard(c, deckPosition, finalPos);
                anim->StartAnimation();
            }

            UpdateOpponentCardsLabel();
            UpdateDeckCountLabel();
        }

        void RedrawOpponentHand() {
            if (opponentHand->Count == 0) {
                UpdateOpponentCardsLabel();
                return;
            }
            int spacing = Math::Min(90, (this->ClientSize.Width - 100) / opponentHand->Count);
            int x = 50;

            for (int i = 0; i < opponentHand->Count; i++) {
                Card^ c = opponentHand[i];
                c->pictureBox->Image = Image::FromFile(
                    "C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\back.png");
                c->pictureBox->Location = deckPosition;
                Point target = Point(x, 50);
                AnimatedCard^ anim = gcnew AnimatedCard(c, deckPosition, target);
                if (!this->Controls->Contains(c->pictureBox))
                    this->Controls->Add(c->pictureBox);
                c->pictureBox->BringToFront();
                anim->StartAnimation();

                x += spacing;
            }
            UpdateOpponentCardsLabel();
        }

        void EndTurn_Click(Object^ sender, EventArgs^ e) {
            for each(Tuple<Card^, Card^> ^ pair in tablePairs) {
                if (pair->Item1 != nullptr) this->Controls->Remove(pair->Item1->pictureBox);
                if (pair->Item2 != nullptr) this->Controls->Remove(pair->Item2->pictureBox);
            }
            tablePairs->Clear();
            tableCards->Clear();

            while (playerHand->Count < 6) {
                Card^ card = DrawFromDeck();
                if (card == nullptr) break;
                playerHand->Add(card);
                card->pictureBox->Location = deckPosition;
                this->Controls->Add(card->pictureBox);
                card->pictureBox->MouseDown += gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                card->pictureBox->MouseMove += gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                card->pictureBox->MouseUp += gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);
                UpdateDeckCountLabel();
            }
            RedrawPlayerHand();
            int oldCount = opponentHand->Count;

            while (opponentHand->Count < 6) {
                Card^ card = DrawFromDeck();
                if (card == nullptr) break;
                opponentHand->Add(card);
                card->pictureBox->Image = Image::FromFile(
                    "C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\back.png");
                card->pictureBox->Location = deckPosition;
                if (!this->Controls->Contains(card->pictureBox))
                    this->Controls->Add(card->pictureBox);
                UpdateDeckCountLabel();
            }
            RedrawOpponentHandIncremental(oldCount);
            UpdateOpponentCardsLabel();
            CheckGameEnd();
            playerTurn = !playerTurn;
            if (!playerTurn) {
                botMoveTimer->Start();
            }
            UpdateButtonStates();
        }

        void Take_Click(Object^ sender, EventArgs^ e) {
            List<Card^>^ takenCards = gcnew List<Card^>();
            for each(Tuple<Card^, Card^> ^ pair in tablePairs) {
                if (pair->Item1 != nullptr) {
                    takenCards->Add(pair->Item1);
                }
                if (pair->Item2 != nullptr) {
                    takenCards->Add(pair->Item2);
                }
            }
            Dictionary<Card^, Point>^ startPos = gcnew Dictionary<Card^, Point>();
            for each(Card ^ c in takenCards) {
                startPos[c] = c->pictureBox->Location;
            }
            tablePairs->Clear();
            tableCards->Clear();
            for each(Card ^ c in takenCards) {
                playerHand->Add(c);
            }
            playerHand->Sort(gcnew Comparison<Card^>(this, &GameWindow::CompareCards));
            SetPlayerHandPositions();
            for each(Card ^ c in takenCards) {
                c->pictureBox->Location = startPos[c];
            }
            for each(Card ^ c in takenCards) {
                Point start = startPos[c];
                Point finalPos = c->originalPosition;
                String^ facePath = safe_cast<String^>(c->pictureBox->Tag);
                c->pictureBox->Image = Image::FromFile(facePath);

                AnimatedCard^ anim = gcnew AnimatedCard(c, start, finalPos);
                anim->StartAnimation();
            }
            for each(Card ^ c in playerHand) {
                c->pictureBox->MouseDown -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                c->pictureBox->MouseMove -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                c->pictureBox->MouseUp -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);

                c->pictureBox->MouseDown += gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                c->pictureBox->MouseMove += gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                c->pictureBox->MouseUp += gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);
            }

            UpdateDeckCountLabel();
            UpdateOpponentCardsLabel();
            RefillBotHand();
            playerTurn = false;
            botMoveTimer->Start();
            UpdateButtonStates();
            CheckGameEnd();
        }


        void RedrawPlayerHand() {
            if (playerHand->Count == 0) return;
            int cardCount = playerHand->Count;
            int spacing;
            int rightBoundary = takeButton->Location.X - 20;
            int maxWidth = rightBoundary - 50;

            if (cardCount <= 6) {
                spacing = 90;
            }
            else {
                spacing = Math::Max(40, maxWidth / cardCount);
            }

            int totalWidth = spacing * (cardCount - 1) + 80;
            int x = 50;

            if (totalWidth < maxWidth) {
                x = (maxWidth - totalWidth) / 2 + 50;
            }

            for (int i = 0; i < cardCount; i++) {
                Point target = Point(x, this->ClientSize.Height - 240);
                playerHand[i]->originalPosition = target;

                if (!this->Controls->Contains(playerHand[i]->pictureBox))
                    this->Controls->Add(playerHand[i]->pictureBox);

                AnimatedCard^ anim = gcnew AnimatedCard(playerHand[i], playerHand[i]->pictureBox->Location, target);
                anim->StartAnimation();

                playerHand[i]->pictureBox->MouseDown -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                playerHand[i]->pictureBox->MouseMove -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                playerHand[i]->pictureBox->MouseUp -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);

                playerHand[i]->pictureBox->MouseDown += gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                playerHand[i]->pictureBox->MouseMove += gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                playerHand[i]->pictureBox->MouseUp += gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);

                x += spacing;
            }
        }

        void TakeCards(List<Card^>^ fromTable, List<Card^>^ toHand) {
            for each(Card ^ card in fromTable) {
                toHand->Add(card);
            }
            fromTable->Clear();
        }

        bool ThrowInCards(List<Tuple<Card^, Card^>^>^ tablePairs, List<Card^>^ botHand, System::Windows::Forms::Form^ container) {
            List<Rank>^ attackRanks = gcnew List<Rank>();
            for each(auto pair in tablePairs) {
                attackRanks->Add(pair->Item1->rank);
            }

            for each(Card ^ card in botHand) {
                if (attackRanks->Contains(card->rank)) {
                    tablePairs->Add(gcnew Tuple<Card^, Card^>(card, nullptr));
                    botHand->Remove(card);

                    card->pictureBox->Image = Image::FromFile((String^)card->pictureBox->Tag);
                    container->Controls->Add(card->pictureBox);

                    Point tableCenter = Point(this->ClientSize.Width / 2 - 75, this->ClientSize.Height / 2 - 60);
                    Point target = Point(tableCenter.X + (tablePairs->Count * 30), tableCenter.Y);
                    AnimatedCard^ anim = gcnew AnimatedCard(card, card->pictureBox->Location, target);
                    anim->StartAnimation();
                    card->pictureBox->BringToFront();

                    UpdateOpponentCardsLabel();
                    return true;
                }
            }
            return false;
        }

        void AnimateBotCardMove(Card^ card, Point to) {
            AnimatedCard^ anim = gcnew AnimatedCard(card, card->pictureBox->Location, to);
            this->Controls->Add(card->pictureBox);
            anim->StartAnimation();
        }

        void BotMove(List<Card^>^ botHand, List<Card^>^ tableCards, Suit trumpSuit) {
            botHand->Sort(gcnew Comparison<Card^>(this, &GameWindow::CompareCards));

            int idx = tablePairs->Count - 1;
            if (idx >= 0 && tablePairs[idx]->Item2 == nullptr) {
                Card^ attack = tablePairs[idx]->Item1;
                Card^ defendCard = nullptr;

                for each(Card ^ c in botHand) {
                    if (CanBeat(attack, c, trumpSuit)) {
                        defendCard = c;
                        break;
                    }
                }

                if (defendCard != nullptr) {
                    tablePairs[idx] = gcnew Tuple<Card^, Card^>(attack, defendCard);
                    botHand->Remove(defendCard);
                    defendCard->pictureBox->Image = Image::FromFile((String^)defendCard->pictureBox->Tag);
                    this->Controls->Add(defendCard->pictureBox);
                    AnimatedCard^ anim = gcnew AnimatedCard(defendCard,
                        defendCard->pictureBox->Location,
                        Point(attack->pictureBox->Left + 10, attack->pictureBox->Top + 10));
                    anim->StartAnimation();
                    defendCard->pictureBox->BringToFront();
                    playerTurn = true;

                    UpdateOpponentCardsLabel();
                    return;
                }
                else {
                    for each(auto p in tablePairs) {
                        if (p->Item1) {
                            botHand->Add(p->Item1);
                            this->Controls->Remove(p->Item1->pictureBox);
                        }
                        if (p->Item2) {
                            botHand->Add(p->Item2);
                            this->Controls->Remove(p->Item2->pictureBox);
                        }
                    }
                    tablePairs->Clear();
                    tableCards->Clear();

                    RefillBotHand();

                    RefillPlayerHand();

                    playerTurn = true;
                    UpdateOpponentCardsLabel();
                    return;
                }
            }
            if (tablePairs->Count == 0 && botHand->Count > 0) {
                Card^ attackCard = botHand[0];
                botHand->RemoveAt(0);

                tablePairs->Add(gcnew Tuple<Card^, Card^>(attackCard, nullptr));
                tableCards->Add(attackCard);

                this->Controls->Remove(attackCard->pictureBox);
                attackCard->pictureBox->Image = Image::FromFile((String^)attackCard->pictureBox->Tag);
                attackCard->pictureBox->Location = deckPosition;
                this->Controls->Add(attackCard->pictureBox);
                attackCard->pictureBox->BringToFront();
                Point tableCenter = Point(this->ClientSize.Width / 2 - 75, this->ClientSize.Height / 2 - 60);
                Point atkPos = Point(tableCenter.X + tableCards->Count * 30, tableCenter.Y);
                AnimatedCard^ anim = gcnew AnimatedCard(attackCard, deckPosition, atkPos);
                anim->StartAnimation();

                isDefending = true;
                lastAttackCard = attackCard;
                playerTurn = true;

                UpdateOpponentCardsLabel();
                return;
            }
        }

        void Card_MouseDown(Object^ sender, MouseEventArgs^ e) {
            PictureBox^ pb = safe_cast<PictureBox^>(sender);
            for each(Card ^ card in playerHand) {
                if (card->pictureBox == pb) {
                    draggedCard = card;
                    mouseOffset = Point(e->X, e->Y);
                    break;
                }
            }
        }

        void Card_MouseMove(Object^ sender, MouseEventArgs^ e) {
            if (draggedCard != nullptr && e->Button == System::Windows::Forms::MouseButtons::Left) {
                draggedCard->pictureBox->BringToFront();
                draggedCard->pictureBox->Left += e->X - mouseOffset.X;
                draggedCard->pictureBox->Top += e->Y - mouseOffset.Y;
            }
        }

        void RefillPlayerHand() {
            int prevCount = playerHand->Count;

            while (playerHand->Count < 6) {
                Card^ card = DrawFromDeck();
                if (card == nullptr)
                    break;
                playerHand->Add(card);

                card->pictureBox->Location = deckPosition;
                this->Controls->Add(card->pictureBox);
            }

            SetPlayerHandPositions();

            for (int i = prevCount; i < playerHand->Count; i++) {
                Card^ c = playerHand[i];
                Point finalPos = c->originalPosition;
                c->pictureBox->Location = deckPosition;

                String^ facePath = safe_cast<String^>(c->pictureBox->Tag);
                c->pictureBox->Image = Image::FromFile(facePath);
                AnimatedCard^ anim = gcnew AnimatedCard(c, deckPosition, finalPos);
                anim->StartAnimation();
            }
            for each(Card ^ c in playerHand) {
                c->pictureBox->MouseDown -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                c->pictureBox->MouseMove -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                c->pictureBox->MouseUp -= gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);

                c->pictureBox->MouseDown += gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                c->pictureBox->MouseMove += gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                c->pictureBox->MouseUp += gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);
            }

            UpdateDeckCountLabel();
        }


        void Card_MouseUp(Object^ sender, MouseEventArgs^ e) {
            if (draggedCard == nullptr) return;

            if (isDefending) {
                if (CanBeat(lastAttackCard, draggedCard, trumpSuit)) {
                    int overlap = 10;
                    Point defendPosition = Point(lastAttackCard->pictureBox->Left + overlap, lastAttackCard->pictureBox->Top + overlap);
                    draggedCard->pictureBox->Location = defendPosition;
                    tablePairs[tablePairs->Count - 1] =
                        gcnew Tuple<Card^, Card^>(lastAttackCard, draggedCard);
                    tableCards->Add(draggedCard);
                    playerHand->Remove(draggedCard);

                    this->Controls->Add(draggedCard->pictureBox);
                    draggedCard->pictureBox->BringToFront();

                    isDefending = false;
                    lastAttackCard = nullptr;
                    playerTurn = false;
                    BotMove(opponentHand, tableCards, trumpSuit);
                    if (!ThrowInCards(tablePairs, opponentHand, this)) {
                        EndTurn_Click(nullptr, nullptr);
                    }
                    else {
                        RedrawOpponentHand();
                        isDefending = true;
                        lastAttackCard = tablePairs[tablePairs->Count - 1]->Item1;
                        playerTurn = true;
                    }

                    UpdateOpponentCardsLabel();
                    UpdateButtonStates();
                    return;
                }
                else {
                    AnimatedCard^ returnAnim = gcnew AnimatedCard(
                        draggedCard,
                        draggedCard->pictureBox->Location,
                        draggedCard->originalPosition);
                    returnAnim->StartAnimation();
                    UpdateButtonStates();
                }
            }
            else {
                Rectangle tableZone = Rectangle(
                    this->ClientSize.Width / 2 - 75,
                    this->ClientSize.Height / 2 - 60,
                    150, 150
                );

                if (draggedCard->pictureBox->Bounds.IntersectsWith(tableZone)) {
                    bool canThrow = false;

                    if (tableCards->Count == 0) {
                        canThrow = true;
                    }
                    else {
                        for each(Card ^ card in tableCards) {
                            if (card->rank == draggedCard->rank) {
                                canThrow = true;
                                break;
                            }
                        }
                        if (!canThrow) {
                            for each(Tuple<Card^, Card^> ^ pair in tablePairs) {
                                Card^ attack = pair->Item1;
                                Card^ defend = pair->Item2;
                                if (attack != nullptr && attack->rank == draggedCard->rank) {
                                    canThrow = true;
                                    break;
                                }
                                if (defend != nullptr && defend->rank == draggedCard->rank) {
                                    canThrow = true;
                                    break;
                                }
                            }
                        }
                    }

                    if (canThrow) {
                        Point tableCenter = Point(this->ClientSize.Width / 2 - 75, this->ClientSize.Height / 2 - 60);
                        Point attackPos = Point(tableCenter.X + tablePairs->Count * 100, tableCenter.Y);
                        draggedCard->pictureBox->Location = attackPos;

                        tablePairs->Add(gcnew Tuple<Card^, Card^>(draggedCard, nullptr));
                        tableCards->Add(draggedCard);
                        playerHand->Remove(draggedCard);

                        this->Controls->Add(draggedCard->pictureBox);
                        draggedCard->pictureBox->BringToFront();

                        Application::DoEvents();

                        if (tableCards->Count > 0) {
                            playerTurn = false;
                            botMoveTimer->Start();
                        }

                        UpdateOpponentCardsLabel();
                        UpdateButtonStates();
                    }
                    else {
                        AnimatedCard^ returnAnim = gcnew AnimatedCard(
                            draggedCard,
                            draggedCard->pictureBox->Location,
                            draggedCard->originalPosition);
                        returnAnim->StartAnimation();
                        UpdateButtonStates();
                    }
                }
            }
            draggedCard = nullptr;
            UpdateButtonStates();
        }

        void StartGame() {
            deck = gcnew Deck();
            playerHand = gcnew List<Card^>();
            opponentHand = gcnew List<Card^>();

            Card^ trump = deck->DrawCard();
            trumpSuit = trump->suit;
            trumpCardData = trump;
            trumpCard = trump->pictureBox;
            trumpCard->Size = System::Drawing::Size(120, 80);
            trumpCard->SizeMode = PictureBoxSizeMode::StretchImage;
            trumpCard->Image->RotateFlip(RotateFlipType::Rotate90FlipXY);
            this->Controls->Add(trumpCard);

            for (int i = 0; i < 6; i++) {
                Card^ playerCard = deck->DrawCard();
                playerHand->Add(playerCard);
                Card^ opponentCard = deck->DrawCard();
                opponentHand->Add(opponentCard);

                opponentCard->pictureBox->Location = deckPosition;
                String^ fileName = opponentCard->rank.ToString() + "_" + opponentCard->suit.ToString() + ".png";
                String^ filepath = "C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\" + fileName;
                opponentCard->pictureBox->Image = Image::FromFile("C:\\Users\\User\\source\\repos\\Dyrakgame\\Dyrakgame\\gamefiles\\back.png");
                opponentCard->pictureBox->Tag = filepath;
                this->Controls->Add(opponentCard->pictureBox);

                playerCard->pictureBox->Location = deckPosition;
                Point cardPos = Point(50 + (i * 90), this->ClientSize.Height - 240);
                playerCard->originalPosition = cardPos;
                this->Controls->Add(playerCard->pictureBox);

                UpdateDeckCountLabel();
            }

            for each(Card ^ card in playerHand) {
                card->pictureBox->MouseDown += gcnew MouseEventHandler(this, &GameWindow::Card_MouseDown);
                card->pictureBox->MouseMove += gcnew MouseEventHandler(this, &GameWindow::Card_MouseMove);
                card->pictureBox->MouseUp += gcnew MouseEventHandler(this, &GameWindow::Card_MouseUp);
            }

            UpdateOpponentCardsLabel();
        }
    };
}