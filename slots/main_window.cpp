/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: main_window.cpp                                             #
# Description: Implements a UI for the game to be playable.         #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/

/* author: YUAN QI
 * student number :267957
 * This file is for implementing the methods, signals, slots in the main_window.hh file
 */

#include "main_window.hh"
#include <QPixmap>
#include <QString>
#include <chrono>
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QLocale>





// To avoid polluting the global namespace with all kinds of
// names, free functions and global variables / constants are
// usually placed inside a namespace. As we don't have a named
// namespace here, we'll use the anonymous namespace instead.
namespace {

/// \brief Get the fruit symbols and their frequencies.
///
/// \return A map of image names and corresponding images and frequencies.
///
Fruits loadFruits() {
    // Set frequencies for each symbol.
    // * You can freely play around with the values below.
    // * Values are relative to each other, higher == more common.
    // * Names of these fruits (and berries too) have to actually match
    //   images found on the disk. The images must be in the PNG format.
    const std::map<std::string, int> frequencies{
        {"cherries", 56},
        {"strawberry", 50},
        {"orange", 46},
        {"pear", 39},
        {"apple", 35},
        {"bananas", 30},
        {"tomato", 24},
        {"grapes", 15},
        {"eggplant", 5}
    };

    // Define where the images can be found and what kind of images they are.
    // * Here we assume that the build directory is next to Slots directory.
    const std::string directory(":/MainWindow/Resources/");
//    const std::string directory(":/slots/Resources/");
    const std::string extension(".png");

    // Load the images into QPixmaps and bundle them with their frequencies.
    Fruits symbols;
    for (const std::pair<std::string, int>& frequency : frequencies) {
        const std::string filename = directory + frequency.first + extension;
        const QPixmap image(QString::fromStdString(filename));
        symbols[frequency.first] = std::make_pair(image, frequency.second);
//        cout<<filename<<endl;
    }

    return symbols;
}

}  // namespace


MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    fruits_(loadFruits()) {
    ui_.setupUi(this);
    initUi();


    ui_.bet_value->setAlignment(Qt::AlignCenter);
    hide_top_Qlabels();
    //set up timer
    timer = new QTimer(this);
    timer->start(1000);

    // connects signals and slot respectively
    connect(ui_.reel_1_lock, SIGNAL(clicked()),this, SLOT(reel_1_lock_licked()));
    connect(ui_.reel_2_lock, SIGNAL(clicked()),this, SLOT(reel_2_lock_licked()));
    connect(ui_.reel_3_lock, SIGNAL(clicked()),this, SLOT(reel_3_lock_licked()));
    connect(ui_.spin_button, SIGNAL(clicked()),this, SLOT(spin_button_clicked()));
    connect(ui_.top_up_button, SIGNAL(clicked()),this, SLOT(top_up_money()));
    connect(ui_.bet_value, SIGNAL(valueChanged(int)),this, SLOT(change_bet()));
    connect(ui_.quit_button, SIGNAL(clicked()),this, SLOT(quit_game()));
    connect(ui_.otto_button, SIGNAL(clicked()),this, SLOT(otto()));
    connect(timer, SIGNAL(timeout()), this ,SLOT(show_current_time()));
    connect(timer, SIGNAL(timeout()), this ,SLOT(show_gaming_time()));
    connect(reel_1,  &Reel::stopped,  this , &MainWindow::reelStopped);
    connect(reel_2,  &Reel::stopped,  this , &MainWindow::reelStopped);
    connect(reel_3,  &Reel::stopped,  this , &MainWindow::reelStopped);




    //set up the text in the comment label
    QString str  ="Welcome to fruit slot machine game!";
    //set up the text in each labels
    show_comment_text(str);
    show_money_and_bonus();
    bonus_cases_info();

    //disable buttons when starting
    all_buttons_states(false);


}

void MainWindow::hide_top_Qlabels()
{

    ui_.reel_3_label_4->setDisabled(true);
    ui_.reel_2_label_4->setDisabled(true);
    ui_.reel_1_label_4->setDisabled(true);
    ui_.reel_3_label_4->hide();
    ui_.reel_2_label_4->hide();
    ui_.reel_1_label_4->hide();

}



////destructor  to delete the dynamicaly memory timer, in case of memory leak
MainWindow::~MainWindow()
{
    delete timer;
    delete reel_1;
    delete reel_2;
    delete reel_3;
}


void MainWindow::initUi() {
    // Initialize and display current funds etc.

    // Set lock buttons to desired starting state.

    // Create one random number generator for all Reels to use.
    // * The seed value is obtained via the chrono library and
    //   represents the number of seconds since 1.1.1970.
    // * If you need to debug some pesky error, you can temporarily
    //   replace the seed with some literal. See Minesweeper or
    //   Waterdropgame versions 1 and 2.
    const auto time = std::chrono::high_resolution_clock::now();
    const auto seed = time.time_since_epoch().count();
    auto rng = std::make_shared<std::default_random_engine>(seed);


    // create a vector for labels
    vector<QLabel*> reel_1_  ={ ui_.reel_1_label_1, ui_.reel_1_label_2, ui_.reel_1_label_3 ,ui_.reel_1_label_4};
    vector<QLabel*> reel_2_ ={ ui_.reel_2_label_1, ui_.reel_2_label_2, ui_.reel_2_label_3 ,ui_.reel_2_label_4};
    vector<QLabel*> reel_3_ ={ ui_.reel_3_label_1, ui_.reel_3_label_2, ui_.reel_3_label_3,ui_.reel_3_label_4 };

    // create unique pointers to take care of the pointers
    reel_1 = new Reel(reel_1_,  ui_.reel_1_lock, &fruits_, rng);
    reel_2 = new Reel(reel_2_,  ui_.reel_2_lock, &fruits_, rng) ;
    reel_3 = new Reel(reel_3_,  ui_.reel_3_lock, &fruits_, rng) ;

}





// slot function handles when spin button licks
void MainWindow::spin_button_clicked()
{


    bool buttons_locked_ = buttons_locked();
    if( not buttons_locked_){

        //handles all locks are locked or not
        if(not all_locks_locked()){
              int bet_money = ui_.bet_value->value();
              total_money -=  bet_money;
        }else{
            QString warning= "Sorry! Do not lock three locks cocurrently!";
            notification(warning);
            reel_1->reset_lock_state();
            reel_2->reset_lock_state();
            reel_3->reset_lock_state();
            change_lock_state(ui_.reel_1_lock);
            change_lock_state(ui_.reel_2_lock);
            change_lock_state(ui_.reel_3_lock);
            return ;
        }
        //spin the reel and set the pics
        reel_1->spin_the_reel();
        reel_2->spin_the_reel();
        reel_3->spin_the_reel();
    }
}



// reel_stopped receives the signal when reel stops , it will do money bonus calculation and show the money
void MainWindow::reelStopped(const vector<string> &fruits)
{

//    cout<<fruits[0]<<" - "<<fruits[1]<<" - "<<fruits[2]<<" - "<<endl;
    fruits.size();
   //after spin change the states of lock button
   if(reel_1->lock_pressed()){
       reel_1->reset_lock_state();
       change_lock_state(ui_.reel_1_lock);
   }
   if(reel_2->lock_pressed()){
       reel_2->reset_lock_state();
       change_lock_state(ui_.reel_2_lock);
   }
   if(reel_3->lock_pressed()){
       reel_3->reset_lock_state();
       change_lock_state(ui_.reel_3_lock);
   }

   //hide the top labels again

   //calculation the bonus
   hide_top_Qlabels();
   bonus_calculation();
   //display the total money after each spin
   show_money_and_bonus();



}

// this function handles all cases to get bonus
// horizontal_paylines and diagonal_paylines
//also record the best bonus you get ever
void MainWindow::bonus_calculation()
{
    const map<string, int> odds_table ={{"cherries",1},{"strawberry",2},{"orange",3},{"pear",4},{"apple",5},
                                         {"bananas",6},{"tomato",7},{"grapes",8},{"eggplant",9}};
    int diagonal_bonus = diagonal_paylines(odds_table);
    int horizontal_bonus =horizontal_paylines(odds_table );
    int bonus = horizontal_bonus+ diagonal_bonus;
    if(bonus>best_bonus){
      best_bonus = bonus ;
    }
    if( bonus> 0){
    QString str = "You are luck! You won "+QString::number(bonus)+" euros in this round!";
    show_comment_text(str);
    }else{
    QString str = "Sorry! You are not lucky enough! Cheers up!";
     show_comment_text(str);
    }
}


//this checks if the fruits names are same and add the bonus
void MainWindow::bonus_case_handler(const string &str_1, const string &str_2, const string &str_3, const map<string, int> &odds_table,int& bonus)
{
   //  cout<<"checking:"<<str_1<<" "<<str_2<<" "<<str_3<<endl;
    if( (str_1.compare(str_2) == 0) and (str_1.compare(str_3) == 0)){
         auto it = odds_table.find(str_1);
         int bet = ui_.bet_value->value();
         int odds = it->second;
         bonus += odds*bet;
         total_money+=bonus;
    }
}

// this function hadles three horizontal paylines  , return bonus you get
int MainWindow::horizontal_paylines(const map<string, int>& odds_table )
{

    int bonus = 0;
    for(size_t i = 0; i< 3;i++){
       string fruit_1 = reel_1->fruit_names[i];
       string fruit_2 = reel_2->fruit_names[i];
       string fruit_3 = reel_3->fruit_names[i];

      bonus_case_handler(fruit_1, fruit_2,fruit_3,odds_table,bonus);
    }
    return bonus;
}


// this function hadles three diagonal paylines  , return bonus you get
int MainWindow::diagonal_paylines(const map<string, int> &odds_table)
{
    int bonus = 0 ;
    string fruit_1 = reel_1->fruit_names[0];
    string fruit_2 = reel_2->fruit_names[1];
    string fruit_3 = reel_3->fruit_names[2];
    bonus_case_handler(fruit_1, fruit_2,fruit_3,odds_table,bonus);

    string fruit_1_ = reel_1->fruit_names[2];
    string fruit_2_ = reel_2->fruit_names[1];
    string fruit_3_ = reel_3->fruit_names[0];
    bonus_case_handler(fruit_1_, fruit_2_,fruit_3_,odds_table,bonus);
    return bonus;

}


//this function checks if the money is 0 , if it is ,disable locks and spin buttons, also gives alert message, return fasle;
//it checks if the money is enough to play next bet, if not, disable the buttons,  ask to top up and return false
//if everything is ok, keep button enabled and return true;
bool MainWindow::buttons_locked()
{
    bool locked = true ;
    bool not_enough_money = not_enough_money_to_bet();
   if(total_money == 0){
        all_buttons_states(false);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Test", "Sorry! You have no money left. Quit?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
        QString warning = "Gave Over";
        notification(warning);
        quit_game();
        } else {
        const QString warning = "Please top up money";
          notification(warning);
        }
   }else if( not_enough_money){
        all_buttons_states( false);
        QString warning =" You do not have enough money to bet! Please top up";
        notification(warning);
   }else {
          all_buttons_states(true);
         locked = false;
    }

   return locked;

}
// check if the money left is enough for next bet
//it compares the bet value and total_money you still have
//return a bool value
bool MainWindow::not_enough_money_to_bet()
{
    return total_money < ui_.bet_value->value();
}


// all_buttons_states could disable and enable 3 locks and spin button
void MainWindow::all_buttons_states(bool locked )
{
        ui_.spin_button->setEnabled(locked);
        ui_.reel_1_lock->setEnabled(locked);
        ui_.reel_2_lock->setEnabled(locked);
        ui_.reel_3_lock->setEnabled(locked);
}
//this function takes care of changing the states of lock button (unlocked or locked)
//also set the text for each case, it calls the methods inside class Reel
void MainWindow::change_lock_state(QPushButton* button){
    QString text = button->text();
    QString new_text;
    const QString locked = "LOCKED";
    const QString unlocked = "UNLOCKED";
    if(text == unlocked){
        new_text = locked ;
        if(button == reel_1->get_lock_button()){
            reel_1->reset_lock_state();
        }else if( button == reel_2->get_lock_button() ){
            reel_2->reset_lock_state();

        }else if(button == reel_3->get_lock_button()) {
              reel_3->reset_lock_state();
        }
    }else if(text == locked){
        new_text = unlocked;
    }
    button->setText(new_text);
}

// warning fucntion to alert user certain message, it takes one QString parameter
void MainWindow::notification(const QString& str)
{
    QMessageBox::warning(this, "warning", str);
}



//this functions return a bool which indicates if all the lock buttons are pressed
bool MainWindow::all_locks_locked()
{
    return (reel_1->lock_pressed() and reel_2->lock_pressed()  and reel_3->lock_pressed());
}


//slot functoin which takes change the state of lock of reel_1
void MainWindow::reel_1_lock_licked()
{
    change_lock_state(ui_.reel_1_lock);
}

//slot functoin which takes change the state of lock of reel_2
void MainWindow::reel_2_lock_licked()
{
    change_lock_state(ui_.reel_2_lock);

}
//slot functoin which takes change the state of lock of reel_3
void MainWindow::reel_3_lock_licked()
{
    change_lock_state(ui_.reel_3_lock);
}






// this function handles topping up money cases
void MainWindow::top_up_money()
{
    int top_up=ui_.top_up_value->text().toInt();;
    if(top_up>0){
        total_top_up +=top_up;
        total_money += top_up;
        ui_.top_up_value->setText(" ");
        all_buttons_states(true);
    }else if(top_up <= 0){
       const  QString warning = " Dont press for fun!";
         notification(warning);
    }
    show_money_and_bonus();
}

// show the total money  and best bonus ever in label
void MainWindow::show_money_and_bonus()
{
      QString bonus = QString::number(best_bonus);
      ui_.best_bonus_->setText(bonus);
      ui_.best_bonus_->setAlignment(Qt::AlignCenter);
      QString money = QString::number(total_money);
       ui_.total_money->setText(money);
       ui_.total_money->setAlignment(Qt::AlignCenter);
    //  cout<<"Total money :"<<total_money<<endl;

}


//this func handles changing bet money and disable buttons if bet is bigger than total money
//enable buttons when bet is less or equal money you have
void MainWindow::change_bet()
{

    int bet = ui_.bet_value->value();
    if( bet > total_money ){
      const  QString warning = "Sorry , you have "+QString::number(total_money) + " euros at most!";
      notification(warning);
      all_buttons_states(false);
    }else{
       all_buttons_states(true);
    }
    ui_.bet_value->setAlignment(Qt::AlignCenter);

}


// handles the cases you quit the game , tell the resut of game and you best bonus
void MainWindow::quit_game()
{

     int profit = total_otto + total_money - total_top_up ;
     QString warning ="You either won or lost! But you lost time!";
     QString best_bonus_= "You best bonus was "+ QString::number(best_bonus)+ " euros";
    if(profit > 0){
    warning  = "You won "+ QString::number(profit) + " euros!";
    }else if(profit< 0){
     warning = "You lost " + QString::number(abs(profit)) + " euros!";
    }
    notification(best_bonus_);
    notification(warning);
    QApplication::quit();
}


// this function handles the cases when you take money out
void MainWindow::otto()
{
    int otto = ui_.otto_money->text().toInt();
    if( otto<= total_money and otto > 0){
        total_otto+=otto;
        total_money -=otto;
        show_money_and_bonus();
    }else if( otto <= 0){
       const QString warning = " Dont press for fun!";
         notification(warning);
    }
     else{
        const QString warning = "Sorry , you have "+QString::number(total_money) + " euros at most!";
          notification(warning);
    }
}

// shows the current time
void MainWindow::show_current_time()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");
    ui_.current_time->setText(time_text);
    ui_.current_time->setAlignment(Qt::AlignCenter);
}

// this one handles the max time you can play otherwise it quits
void MainWindow::show_gaming_time()
{
      const int  min_ = 5 ;
      const int  sec_ = 1 ;
      ui_.sec_display->display(sec);
      ui_.min_display->display(min);
      sec += 1;
      if (sec == 60)
      {
          min += 1;
          sec = 0;
      }
      if(sec == sec_ and  min == min_){
         const QString warning = "You have played enough time! Please quit the game!";
          notification(warning);
          quit_game();
      }
}


//after each spin, it  give you info about the result of that spin
void MainWindow::show_comment_text(QString& str)
{
    ui_.comment->setText(str);
    ui_.comment->setAlignment(Qt::AlignCenter);
}




//this func takes care that it shows all the possib7ilities of winning bonus on the top of window
void MainWindow::bonus_cases_info()
{
    string apple ="apple";
    set_info_pix(apple, ui_.apple_1);
    set_info_pix(apple, ui_.apple_2);
    set_info_pix(apple, ui_.apple_3);

    string eggplant ="eggplant";
    set_info_pix(eggplant, ui_.eggplant_1);
    set_info_pix(eggplant, ui_.eggplant_2);
    set_info_pix(eggplant, ui_.eggplant_3);

    string grapes ="grapes";
    set_info_pix(grapes, ui_.grapes_1);
    set_info_pix(grapes, ui_.grapes_2);
    set_info_pix(grapes, ui_.grapes_3);


    string tomato ="tomato";
    set_info_pix(tomato, ui_.tomato_1);
    set_info_pix(tomato, ui_.tomato_2);
    set_info_pix(tomato, ui_.tomato_3);

    string bananas ="bananas";
    set_info_pix(bananas, ui_.bananas_1);
    set_info_pix(bananas, ui_.bananas_2);
    set_info_pix(bananas, ui_.bananas_3);


    string pear ="pear";
    set_info_pix(pear, ui_.pear_1);
    set_info_pix(pear, ui_.pear_2);
    set_info_pix(pear, ui_.pear_3);

    string orange ="orange";
    set_info_pix(orange, ui_.orange_1);
    set_info_pix(orange, ui_.orange_2);
    set_info_pix(orange, ui_.orange_3);

    string strawberry ="strawberry";
    set_info_pix(strawberry, ui_.strawberry_1);
    set_info_pix(strawberry, ui_.strawberry_2);
    set_info_pix(strawberry, ui_.strawberry_3);

    string cherries ="cherries";
    set_info_pix(cherries, ui_.cherries_1);
    set_info_pix(cherries, ui_.cherries_2);
    set_info_pix(cherries, ui_.cherries_3);


}




// this fucntion loads picture for bonus-cases instruction sitting on the top of main window
void MainWindow::set_info_pix(string &name, QLabel *label)
{
    QPixmap pic = fruits_.at(name).first;
    int w = label->width();
    int h = label->height();
    label->setPixmap(pic.scaled(w,h,Qt::KeepAspectRatio));
    label->setAlignment(Qt::AlignCenter);
}






