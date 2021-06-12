#include "GameGUI.h"

//Will set tile to yellow color background to player score
void Window::PlayerScoreYellowTile(){
    PlayerScore_->setStyleSheet("background-color : yellow ; font-size : 22 px; font-weight : bold; color : black");
}

//Func will set tile to green color background to player score
void Window::PlayerScoreDefaultTile(){
    PlayerScore_->setStyleSheet("background-color : black ; font-size : 20 px; font-weight : bold; color : white");
}

//Will set tile to yellow color background to dealer score
void Window::DealerScoreYellowTile(){
    DealerScore_->setStyleSheet("background-color : yellow ; font-size : 22 px; font-weight : bold; color : black");
}

//Func will set tile to green color background to dealer score
void Window::DealerScoreDefaultTile(){
    DealerScore_->setStyleSheet("background-color : black ; font-size : 20 px; font-weight : bold; color : red");
}

//Func to blink score tile when player's score is updated
void Window::PlayerScoreUpdateBlink(){
    PlayerScoreYellowTile();
    QTimer::singleShot(300,this,&Window::PlayerScoreDefaultTile);
    QTimer::singleShot(600,this,&Window::PlayerScoreYellowTile);
    QTimer::singleShot(900,this,&Window::PlayerScoreDefaultTile);

}

//Func to blink score tile when dealer's score is updated
void Window::DealerScoreUpdateBlink(){
    DealerScoreYellowTile();
    QTimer::singleShot(300,this,&Window::DealerScoreDefaultTile);
    QTimer::singleShot(600,this,&Window::DealerScoreYellowTile);
    QTimer::singleShot(900,this,&Window::DealerScoreDefaultTile);

}

//Func to change Current bet as the value of spin box being changed
void Window::ChangeBet(int NewBetValue){
    Window::CurrentBet_ = NewBetValue;
}

//Func to reduce the players fund once bet has been placed
void Window::ReducePlayersFundForBet(){
    PlayersFund_-= CurrentBet_;
    RefreshPlayersFund();
}

//Func to refresh the current bet shown in window
void Window::RefreshCurrentBet(){
    CurrentBetInfoLabel_->setNum(CurrentBet_);
}

//Func to refresh the current score of dealer shown in window
void Window::RefreshDealerScore(){
    DealerScore_->setNum(Dealer_->TotalScore_);
}

//Func to refresh the current score of dealer shown in window
void Window::RefreshPlayerScore(){
    PlayerScore_->setNum(Player_->TotalScore_);
}

//Refreshing Player Fund shown in the window
void Window::RefreshPlayersFund(){
    PlayersFundInfoLabel_->setNum(PlayersFund_);
}

//Refreshing Dealers Fund shown in the window
void Window::RefreshDealersFund(){
    DealersFundInfoLabel_->setNum(DealersFund_);
}

//Update the fund for player and dealer if player has blackjack
void Window::PlayerHasBlackJack(){
    //since we have already reduced the bet amount from player if, it won
    // we will have to add 2.5 times of it
    PlayersFund_+= CurrentBet_*2.5;
    DealersFund_-= CurrentBet_*1.5;
    RefreshPlayersFund();
    RefreshDealersFund();
    QTimer::singleShot(1000,this,&Window::ResetGame);
}

//Update the fund for player and dealer if player won
void Window::PlayerWon(){
    PlayersFund_+= CurrentBet_*2;
    DealersFund_-= CurrentBet_;
    RefreshPlayersFund();
    RefreshDealersFund();
    QTimer::singleShot(1000,this,&Window::ResetGame);
}

//Update the fund for player and dealer if player lost
void Window::PlayerLost(){
    //we have already taken money from dealer so no need to deduct it anymore
    DealersFund_+= CurrentBet_;
    RefreshPlayersFund();
    RefreshDealersFund();
    QTimer::singleShot(1000,this,&Window::ResetGame);
}

//Update the fund for player and dealer if game is draw
void Window::GameDraw(){
    PlayersFund_+= (CurrentBet_);
    RefreshPlayersFund();
    QTimer::singleShot(1000,this,&Window::ResetGame);
}

//Hiding the first best prompt
void Window::HideFirstBetPrompt(){
    FirstBetPrompt_->setVisible(false);
}

//showing first bet prompt
void Window::ShowFirstBetPrompt(){
    FirstBetPrompt_->setVisible(true);
}

//Hide Hit and Stay prompt
void Window::HideHitNStayPrompt(){
    HitNStayPrompt_->setVisible(false);
}

//Show Hit and Stay prompt
void Window::ShowHitNStayPrompt(){
    HitNStayPrompt_->setVisible(true);
}

//Hide message box from the main window
void Window::HideMessageBoxPrompt(){
    MessageBoxPrompt_->setVisible(false);
}

//show message box on the main window
void Window::ShowMessageBoxPrompt(){
    MessageBoxPrompt_->setVisible(true);
}

//Show players card and check the score if its nore more than 21, if it is reset the game
void Window::ShowPlayersCard(){
    
    Player_->RevealNextCard(); //first revealing the new card
    QTimer::singleShot(TimeInBetweenCards_,this,&Window::RefreshPlayerScore);//Refreshing the score value shown in window
    //If now score is more than 21 Player is burst
    if ((Player_->TotalScore_) > 21){//Player loses if the score is more than 21 and resets the game
        QTimer::singleShot(TimeInBetweenCards_,this,&Window::PlayerLost); 
    }
}

//Show dealers card and check if its not more than 21, if it is reset the game
void Window::ShowDealersCard(){
    
    Dealer_->RevealNextCard();//First revealing the new card
    QTimer::singleShot(TimeInBetweenCards_,this,&Window::RefreshDealerScore); //Refreshing the score value shown in window
    }

//Check if first two cards dealt has black jack for player, if not move on the game
void Window::CheckIfBlackJack(){
    if(Player_->TotalScore_ ==21){
        QTimer::singleShot(TimeInBetweenCards_,this,&Window::PlayerHasBlackJack);
    }
    else{
        StartTableSetupDealer();
    }
}

//Setting up table and revealing two cards
void Window::StartTableSetupPlayer(){
    
    ShowPlayersCard();//Showing players first two cards
    QTimer::singleShot(TimeInBetweenCards_,this,&Window::ShowPlayersCard);
    QTimer::singleShot(TimeInBetweenCards_*2,this,&Window::CheckIfBlackJack);
    //Checking if player has black jack if not move on to set up table for dealer!
}

//Setting up table and revealing first and booking second closed card
void Window::StartTableSetupDealer(){
    ShowDealersCard();
    QTimer::singleShot(TimeInBetweenCards_,this,&Window::ShowDealersCard);
}

//Compare delaer and palyer score and declare win/lose
void Window::CompareScoresAndMoveOn(){
    if ((Dealer_->TotalScore_) > 21 || (Dealer_->TotalScore_)<(Player_->TotalScore_)){
        QTimer::singleShot(TimeInBetweenCards_,this,&Window::PlayerWon);
        }
    else if((Dealer_->TotalScore_)>(Player_->TotalScore_)){
        QTimer::singleShot(TimeInBetweenCards_,this,&Window::PlayerLost);
        }
    else if((Dealer_->TotalScore_)==(Player_->TotalScore_)){
        QTimer::singleShot(TimeInBetweenCards_,this,&Window::GameDraw);
        }
}
//Func to reset game once the game is done
void Window::ResetGame(){
    Dealer_->ResetCards();
    Player_->ResetCards();
    HideHitNStayPrompt();
    ShowFirstBetPrompt();
    RefreshDealerScore();
    RefreshPlayerScore();
}

//Turning the close card of the dealer
void Window::TurnDealersSecondCard(){
    Dealer_->FlipDealersCard();
    QTimer::singleShot(TimeInBetweenCards_,this,&Window::RefreshDealerScore); //Refreshing the score value shown in window
}

//Ending the game when player press "Stay" button
//We will first Check if the Dealers point is 17 or above
//if not than we reveal next cards untill we reach above 16
//After that we compare the scores
void Window::DealersTurn(){
    TurnDealersSecondCard();
    QTimer::singleShot(TimeInBetweenCards_*2.5,this,&Window::EndGame);
}

void Window::EndGame(){
    if(Dealer_->TotalScore_<= 16){
        while(Dealer_->TotalScore_<= 16){
            ShowDealersCard();
            }
        }
    QTimer::singleShot(TimeInBetweenCards_,this,&Window::CompareScoresAndMoveOn);
}
    
void Window::StartFirstGame(){
    Window::HideHitNStayPrompt(); //Hiding the Hit and Stay prompt, 
    Window::HideMessageBoxPrompt(); //Hiding the Message box showing winning and loosing of the game
    //we only need first bet while we ask user for the bet they want to put

    //now we will connect the spin box to the current bet so that changing it will change currentBet variable
    connect(BetBox_, qOverload<int>(&QSpinBox::valueChanged), this, &Window::ChangeBet);

    //Next we will connect okay button where we will show Players fund reduced when bet is placed
    connect(OkButton_,&QPushButton::clicked, this, &Window::ReducePlayersFundForBet);
    connect(OkButton_,&QPushButton::clicked, this, &Window::RefreshCurrentBet);
    connect(OkButton_,&QPushButton::clicked, this, &Window::HideFirstBetPrompt);
    connect(OkButton_,&QPushButton::clicked, this, &Window::ShowHitNStayPrompt);
    connect(OkButton_,&QPushButton::clicked, this, &Window::StartTableSetupPlayer);

    //Hit and stay button
    connect(HitButton_,&QPushButton::clicked, this, &Window::ShowPlayersCard);
    connect(StayButton_,&QPushButton::clicked, this, &Window::DealersTurn);
    //timer to time the first table setup of the game
}