#include <simplecpp>
#include<string>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */

const int LEFT_MARGIN = 70;

double dist(double x1, double y1, double x2, double y2)//This function returns the distance between two points
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

double abso(double x)//It returns the absolute value of a function
{
    if (x >= 0) return x;
    else return -x;
}

void print_results(int score, int time)//This function prints match score and time taken.
{
    char s[10] = "Score : _";
    s[8] = score+48;
    Text Score(WINDOW_X/2, BOTTOM_MARGIN/2, s);
    char tim[16] = "Time Taken : 00";
    tim[14] = time%10 + 48;
    tim[13] = time/10 + 48;
    Text Time(WINDOW_X/2, WINDOW_Y/2, tim);
    wait(5);
}

bool Bullet_X_bubble(Bullet bt, Bubble bb)//This function returns if bubble and bullet colloid
{
    double btx = bt.get_center_x(), bty = bt.get_center_y(), btw = bt.get_width(), bth = bt.get_height(),
            bbx = bb.get_center_x(), bby = bb.get_center_y(), bbr = bb.get_radius();
    if ((dist(bbx, bby, btx+btw/2, bty+bth/2) < bbr)
        ||
        (dist(bbx, bby, btx-btw/2, bty+bth/2) < bbr)
        ||
        (dist(bbx, bby, btx-btw/2, bty+bth/2) < bbr)
        ||
        (dist(bbx, bby, btx-btw/2, bty-bth/2) < bbr)) return true;
    else return false;
}

bool bubble_X_shooter(Shooter &shooter, vector<Bubble> &bubbles)//This functions returns true if bubble collides with shooter
{
    double shhcx = shooter.get_head_center_x(), shhcy = shooter.get_head_center_y(), shhr = shooter.get_head_radius();
    for(unsigned int i = 0; i < bubbles.size(); i++)
    {
        double bbx = bubbles[i].get_center_x(), bby = bubbles[i].get_center_y(), bbr = bubbles[i].get_radius();
        if (dist(bbx, bby, shhcx, shhcy) < bbr+shhr)//collision of shooter head and bubble
        {
            if (bbr > BUBBLE_DEFAULT_RADIUS) shooter.bubble_hit(true);
            else shooter.bubble_hit(false);
            bubbles.erase(bubbles.begin()+i);
            return true;
        }
        double btx = shooter.get_body_center_x(), bty = shooter.get_body_center_y(),
            btw = shooter.get_body_width(), bth = shooter.get_body_height();
        if ((dist(bbx, bby, btx+btw/2, bty+bth/2) < bbr)
            ||
            (dist(bbx, bby, btx-btw/2, bty+bth/2) < bbr)
            ||
            (dist(bbx, bby, btx-btw/2, bty+bth/2) < bbr)
            ||
            (dist(bbx, bby, btx-btw/2, bty-bth/2) < bbr))
            {
                if (bbr > BUBBLE_DEFAULT_RADIUS) shooter.bubble_hit(true);
                else shooter.bubble_hit(false);
                bubbles.erase(bubbles.begin()+i);
                return true;
            }
    }
    return false;
}

void move_bullets(vector<Bullet> &bullets, vector<Bubble> &bubbles, int &score){
    // move all bullets
    for (unsigned int i = 0; i < bullets.size(); i++)//To erase bullet and bubble if they colloide
        for (unsigned int j = 0; j < bubbles.size(); j++)
            if (Bullet_X_bubble(bullets[i], bubbles[j]))
            {
                bullets.erase(bullets.begin() + i);
                if (bubbles[j].get_radius() <= BUBBLE_DEFAULT_RADIUS)//for smaller bubbles
                {
                    bubbles.erase(bubbles.begin() + j);
                }
                else
                {
                    bubbles.push_back(Bubble(bubbles[j].get_center_x()-bubbles[j].get_radius(), bubbles[j].get_center_y(),
                        bubbles[j].get_radius()/2, -2*bubbles[j].get_vx(), bubbles[j].get_vy(), COLOR(255, 105, 180)));
                    bubbles.push_back(Bubble(bubbles[j].get_center_x()+bubbles[j].get_radius(), bubbles[j].get_center_y(),
                        bubbles[j].get_radius()/2, 2*bubbles[j].get_vx(), bubbles[j].get_vy(), COLOR(255, 105, 180)));
                    bubbles.erase(bubbles.begin() + j);
                }
                score ++;
            }
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X*1.0/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X*3.0/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X*1.0/2.0, BUBBLE_START_Y, BIGGER_BUBBLE_RADIUS, BUBBLE_DEFAULT_VX/2, 0, COLOR(255,0,0)));
    return bubbles;
}


int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    Line Top_Margin(0, TOP_MARGIN, WINDOW_X, TOP_MARGIN);

    b1.setColor(COLOR(0, 0, 255));

    int score = 0;//Score is Calculated
    string Scoreprint("Score:_");
    Text Score(WINDOW_X - LEFT_MARGIN, TOP_MARGIN-textheight("s")/2, Scoreprint);
    //Prints score


    int time = 0, no_of_rounds = 0;//no of rounds calculates no of times step time is calculated
    //time calculates time taken
    char Timeprint[8] = "Time:00";
    Text Time(WINDOW_X/2, BOTTOM_MARGIN, Timeprint);
    //prints time

    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    string Healthprint = "Health:_";
    Text Health(LEFT_MARGIN+textwidth("Health:"), TOP_MARGIN-textheight("s")/2, Healthprint);


    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }


        Timeprint[6] = time%10+48;
        Timeprint[5] = time/10+48;
        Time.setMessage(Timeprint);
        //Updates the time

        Healthprint[Healthprint.length()-1] = shooter.health +48;
        Health.setMessage(Healthprint);
        //updates the Health left

        Scoreprint[6] = score+48;
        Score.setMessage(Scoreprint);

        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets, bubbles, score);


        if (bubbles.size() == 0)//That is all the bubbles are destroyed
        {
            Scoreprint[6] = score+48;
            Score.setMessage(Scoreprint);
            print_results(score, time);//Prints score and time taken
            return 0;
        }

        if (bubble_X_shooter(shooter, bubbles))
        {
            if (shooter.health <= 0)
            {
                Text dead(LEFT_MARGIN, WINDOW_Y/2, "Shooter is dead");
                wait(2);
                print_results(score, time);
                return 0;
            }
        }

        if (time >= 80)
        {
            Text time_up(LEFT_MARGIN, WINDOW_Y/2, "Time is up");
            wait(2);
            print_results(score, time);
            return 0;
        }

        no_of_rounds++;
        if (no_of_rounds%50 == 0) time++;//as step time is 0.02 It takes 50 rounds to complete 1 sec.

        wait(STEP_TIME);
    }
}
