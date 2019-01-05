/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: reel.cpp                                                    #
# Description: Draws random fruits for labels to display.           #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/

#include "reel.hh"
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QPainter>
/* author: YUAN QI
 * student number :267957
 * This file is for implementing the methods, signals ,slots for reel class.
 */


//constructor to do some setup and init variables
Reel::Reel(const std::vector<QLabel*>& labels,
           const QPushButton* lock_button,
           const Fruits* fruits,
           std::shared_ptr<std::default_random_engine> rng) {
    // Connect all signals & slts you need here.
    this->labels = labels;
    this->lock_button = lock_button;
    this->fruits = fruits;
    this->rng = rng;
    set_fruits_img();;
    reel_timer = new QTimer(this);
    animation_timer = new QTimer(this);
    connect(reel_timer, SIGNAL(timeout()), this, SLOT(reel_timer_stopped()));
}

Reel::~Reel()
{
    delete animation_timer;
    delete reel_timer;
}


// this fuc does the thing to load the pictures when starting this game and after each spin
void Reel::set_fruits_img(){
    QPixmap pix;
    fruit_names.clear();
    const vector<int> values = {56,50,46,39,35,30,24,15,5};
    std::discrete_distribution<size_t> d ({56,50,46,39,35,30,24,15,5});
    for(auto labels_it = labels.begin(); labels_it!= labels.end(); labels_it++){
       size_t index  = d(*rng);
        for(auto fruits_it = fruits->begin() ; fruits_it != fruits->end(); fruits_it++){
            if ((fruits_it->second).second == values.at(index)){
                pix = fruits_it->second.first;
                fruit_names.push_back(fruits_it->first);
                 break;
            }
        }
        const int w = 30;
        const int h = 30;
        (*labels_it)->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        (*labels_it)->setAlignment(Qt::AlignCenter);

    }

}


// reel_timer_stopped which be triggered after certain ms, and  it disconnects the link between timer and moving_pixes on the reel
//then it takes all the fruits names and transmit from reel obejct to mainwindow class.
void Reel::reel_timer_stopped()
{
   disconnect(animation_timer, SIGNAL(timeout()), this, SLOT(spining_illusion()));
   animation_timer->stop();
   emit stopped(fruit_names);
}





//it handles when lock button is not pressed , change the images
//here I set two timers , one is for the time duration for spinning the reel and another one is deciding when the spinning should be terminated for one round
//reel_timer only call one time and it stops after 4500mss , animatione timer calls very 8ms.
void Reel::spin_the_reel()
{
   if(not locked){
       const int time =5;
       int timer_value =rand()% 1000 + 1500;
//       cout<<timer_value<<endl;
       animation_timer->start(time);
       connect(animation_timer, SIGNAL(timeout()), this, SLOT(spining_illusion()));
       reel_timer->start(timer_value);
       reel_timer->setSingleShot(true);
   }

}



// actually this fucntion does the thing like it moves the QLabels down 8px very 5ms
//the total_move decides how far it should move down in y scale and each time it move 8px whcih is visible to show moving.
// its gonna change the pix and reset the image and restore to the origin place. during the 4500 ms, it repreats many times and mimic the reel is spinning
void Reel::spining_illusion()
{
    const int total_move = 16;
    const int each_step = 8;
    const size_t index = 3 ;
    for( size_t i = 0 ; i <4 ; i++){
         int x = labels[i]->x();
         int y = labels[i]->y();
         y+=each_step;
         if(i == index){
             move++;
         }
       // labels[index]->show();
         labels[index]->show();
         labels[index]->setDisabled(false);
         labels[i]->move(x,y);
         //put back to nonwhere
         if(total_move == move and index == 3){
           move = 0;
           set_fruits_img();
           //show the top qlabels
          }
    }
}


// toggle the lock
void Reel::reset_lock_state(){
    locked = !locked;
}

// return the pointer of lock button
const  QPushButton* Reel::get_lock_button()
{
    return lock_button;
}


// return the status of lock button
bool Reel::lock_pressed(){
    return locked;
}

