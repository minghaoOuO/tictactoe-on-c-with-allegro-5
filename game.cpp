#include "game.hpp"
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

// ¤E®c®æ®y¼Ð nine square division coordination
int xcoord[11] = {650, 870, 1090, 650, 870, 1090, 650, 870, 1090, 0};
int ycoord[11] = {230, 230, 230, 420, 420, 420, 590, 590, 590, 0};

// initialize the game
void game::initial() {
    // initialize allegro variables
    display = NULL;
    background = NULL;
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return;
    }
    al_init_image_addon();  //should check for errors

    display = al_create_display(1920, 1080);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return;
    }
    // print the background
    background = al_load_bitmap("./source/background3.jpg");
    if (!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        return;
    }
    al_draw_bitmap(background, 0, 0, 0);
    // print the images
    ALLEGRO_BITMAP* img = al_load_bitmap("./source/head5.png");
    if (img == NULL) {
        fprintf(stderr, "Load of \"pic\" image failed \n");
        return;
    }
    al_draw_bitmap(img, 700, 200, NULL);
    
    // the nine sqare division image
    ALLEGRO_BITMAP* img2 = al_load_bitmap("./source/12345.png");
    if (img == NULL) {
        fprintf(stderr, "Load of \"pic\" image failed \n");
        return;
    }

    al_draw_bitmap(img2, 650, 750, NULL);

    al_flip_display();


    ALLEGRO_EVENT Event;
    ALLEGRO_EVENT_QUEUE* EventQueue = NULL;

    EventQueue = al_create_event_queue();
    if (EventQueue == NULL)
    {
        return;
    }

    if (!al_install_keyboard()) ///setup the keyboard
    {
        return;
    }
    // choose the mode of the game, 1 : random , 2 : smarter automatically choose
    al_register_event_source(EventQueue, al_get_keyboard_event_source()); ///Tell allegro to get events from the keyboard
    while (true) {
        al_wait_for_event(EventQueue, &Event);
        if (Event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (Event.keyboard.keycode >= ALLEGRO_KEY_1 && Event.keyboard.keycode <= ALLEGRO_KEY_5)
            {
                mode = Event.keyboard.keycode - 27;
                std::cout << mode << std::endl;
                break;
            }
        }
    }
    
    //al_rest(2.0);
    al_destroy_event_queue(EventQueue);
}

// close the game
void game::terminate() {
    al_destroy_display(display);
    al_destroy_bitmap(background);
    al_uninstall_system();
    return;
}

// start the game
void game::start() {
    initial();
    play();
    terminate();
}

// the play scene
void game::play() {
    //display = NULL;
    //background = NULL;
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return;
    }
    al_init_image_addon();  //should check for errors

    if (!display) {
        display = al_create_display(1920, 1080);
        fprintf(stderr, "failed to create display!\n");
        return;
    }
    // display the background
    background = al_load_bitmap("./source/background3.jpg");
    if (!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        return;
    }
    al_draw_bitmap(background, 0, 0, 0);
    ALLEGRO_BITMAP* img = al_load_bitmap("./source/board_v1.png");
    if (img == NULL) {
        fprintf(stderr, "Load of \"pic\" image failed \n");
        return;
    }
    al_draw_bitmap(img, 550, 200, NULL);
    al_flip_display();
    // the OXs images
    ALLEGRO_BITMAP* O = al_load_bitmap("./source/O_v2.jpg");
    ALLEGRO_BITMAP* X = al_load_bitmap("./source/X_v2.jpg");
 
    int turncount = 1;  // to record whose turn
    while (!curgame.checkwin()) {
        int set;
        if (turncount % 2) {    // player's turn
            set = curgame.playerset();
            if (set < 0 && set > 9) continue;
            al_draw_bitmap(O, xcoord[set], ycoord[set], NULL);
        }
        else {                  // com's turn
            set = curgame.comset(mode);
            if (set < 0 && set > 9) continue;
            al_draw_bitmap(X, xcoord[set], ycoord[set], NULL);
        }
        curgame.set(set, curgame.cur_player);
        turncount++;
        al_flip_display();
    }

    al_rest(2.0);
    al_draw_bitmap(background, 0, 0, 0);

    // win lose tie images
    ALLEGRO_BITMAP* win = al_load_bitmap("./source/win_v1.png");
    ALLEGRO_BITMAP* lose = al_load_bitmap("./source/lose_v1.png");
    ALLEGRO_BITMAP* tie = al_load_bitmap("./source/Tie_v1.png");

    if (curgame.checkwin() == 1) {  // win scene
        std::cout << "YOU WIN!!\n";
        al_draw_bitmap(win, 300, 600, NULL);
    }
    else if (curgame.checkwin() == 2) { // lose scene
        al_draw_bitmap(lose, 300, 600, NULL);
        std::cout << "YOU LOSE!!\n";
    }
    else {  // tie scene
        al_draw_bitmap(tie, 300, 600, NULL);
        std::cout << "Tie!! GGWP!" << std::endl;
    }

    al_flip_display();

    al_rest(2.0);

    return;
}

game::game() {
    display = NULL;
    background = NULL;
    curplayer = 1;
    mode = 0;
    return;
}