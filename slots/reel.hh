/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: reel.h                                                      #
# Description: Draws random fruits for labels to display.           #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/
/* author: YUAN QI
 * student number :267957
 * This file defines the reel class
 */

#ifndef SLOTS_REEL_H
#define SLOTS_REEL_H
#include "ui_main_window.h"
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

using namespace  std;
// Fruits is an alias for a map which contains a fruit's name and
// corresponding image and likelyhood of appearance.
using Fruits = std::map<std::string, std::pair<QPixmap, int>>;


/// \class Reel
/// \brief Implements reels which are used to determine if the player has won.
///
/// Reel spins for a random duration and draws random fruits to display on its
/// labels. Reel signals MainWindow with the name of the middlemost fruit, but
/// an ordered (top-to-bottom) list of all fruits can also be queried.
///
class Reel: public QObject {
    Q_OBJECT

public:

    /// \brief Construct a new Reel.
    ///
    /// \param[in] labels Labels which should be used to display the symbols.
    /// \param[in] lock_button Button whose state determines if Reel is locked.
    /// \param[in] fruits Images and frequencies of each fruit.
    /// \param[in] rng Random number generator used to determine the symbols.
    ///
    Reel(const std::vector<QLabel*>& labels,
         const QPushButton* lock_button,
         const Fruits* fruits,
         std::shared_ptr<std::default_random_engine> rng);

   ~Reel();
    void set_fruits_img();
    bool lock_pressed();
    void reset_lock_state();
    const QPushButton* get_lock_button();
    vector<string> fruit_names;
    std::vector<QLabel*> labels;









public slots:
        void spin_the_reel();
        void spining_illusion();
        void reel_timer_stopped();




private:
//    void set_fruits_img();

    const QPushButton* lock_button;
    const Fruits* fruits;
    std::shared_ptr<std::default_random_engine> rng;
    bool locked = false;

    QTimer* animation_timer = nullptr;
    QTimer* reel_timer = nullptr;
    int move= 0 ;








signals:

    /// \brief Is emitted when the Reel stops.
    ///
    /// \param[in] Name of the middle symbol on the Reel.
    ///
    void stopped(const vector<string>& fruits);


};  // class Reel


#endif  // SLOTS_REEL_H
