/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: main_window.h                                               #
# Description: Implements a UI for the game to be playable.         #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/
/* author: YUAN QI
 * student number :267957
 * This file defines the main window class
 */

#ifndef SLOTS_MAINWINDOW_H
#define SLOTS_MAINWINDOW_H
#include "ui_main_window.h"
#include "reel.hh"
#include <QMainWindow>
#include <string>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QDateTime>


using namespace std;


/// \class MainWindow
/// \brief Implements the main window which is used to interact with the game.
///
/// MainWindow owns all the widgets used to display the game to the player
/// and to play the game. MainWindow contains the whole game, no other windows
/// are used.
///
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /// \brief Construct a new MainWindow.
    ///
    /// \param[in] parent Points to this MainWindow's parent widget.
    ///
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void show_money_and_bonus();
    void bonus_cases_info();
    void show_comment_text(QString &str);

private slots:

    /// \brief Check if the round is over and prepare for the next round.
    ///
    /// Should get called when a Reel has decided on which fruits to display.
    ///
    /// \param[in] middle_sym Name of the fruit symbol on the middle lane.
    ///
    void reelStopped(const vector<string>& fruits);
    void reel_1_lock_licked();
    void reel_2_lock_licked();
    void reel_3_lock_licked();
    void spin_button_clicked();
    void top_up_money();
    void change_bet();
    void quit_game();
    void otto();
    void show_current_time();
    void show_gaming_time();








private:

    /// \brief Initialize the Reels, buttons, etc. so the game can be played.
    ///
    void initUi();
    void change_lock_state(QPushButton* button);
    void notification(const QString& str );
    bool all_locks_locked();
    void bonus_calculation();
    void bonus_case_handler(const string& str_1, const string& str_2,const string& str_3,const map<string, int>& odds_table, int& bonus);
    bool not_enough_money_to_bet();
    void all_buttons_states(bool locked);
    bool buttons_locked();
    int horizontal_paylines(const map<string, int>& odds_table);
    int diagonal_paylines(const map<string, int>& odds_table);
    void set_info_pix(string& name, QLabel* label );
    void hide_top_Qlabels();

    const Fruits fruits_;       ///< The game symbols and their rarities.
    Ui::MainWindowClass ui_;    ///< (Black) Qt magic handling the UI.

    Reel* reel_1 =nullptr;
    Reel* reel_2=nullptr;
    Reel* reel_3=nullptr;
    QTimer *timer  =nullptr;


    int total_money = 0 ;
    int total_top_up = 0;
    int total_otto = 0;
    int best_bonus=0;
    int min ;
    int sec;


};  // class MainWindow


#endif  // SLOTS_MAINWINDOW_H
