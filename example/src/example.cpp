#include "progresscpp/ProgressBar.hpp"
#include <iostream>
#include <unistd.h>

/* Example usage of ProgressBar */
auto main() -> int
{
    const int total = 10000;

    /*
     * Define a progress bar that has a total of 10000,
     * a width of 70, shows `#` to indicate completion
     * and a dash '-' for incomplete
     */
    progresscpp::ProgressBar progress_bar0(total, 70);

    for(int i = 0; i < total; i++) {
        ++progress_bar0; // record the tick

        usleep(200); // simulate work

        // display the bar only when there was at least 5% progress
        // since the last time it was displayed
        progress_bar0.display();
    }

    // tell the bar to finish
    progress_bar0.done();


    progresscpp::ProgressBar progress_bar1(total, 70, '#', '-');
    for(int i = 0; i < total / 1.2; i++) {
        ++progress_bar1; // record the tick

        usleep(200); // simulate work

        // display the bar only when there was at least 10% progress
        // since the last time it was displayed
        progress_bar1.displayIfChangedAtLeast(0.1);
    }

    //let the bar fail
    progress_bar1.failure();


    progresscpp::ProgressBar progress_bar2(total, 70, '-');
    for(int i = 0; i < total / 1.2; i++) {
        ++progress_bar2; // record the tick

        usleep(200); // simulate work

        // display the bar only when there was at least 1% progress
        // since the last time it was displayed
        progress_bar2.displayIfChangedAtLeast(0.01);
    }

    progress_bar2.done();

	
    progresscpp::ProgressBar progress_bar3(total, 70);
    for(int i = 0; i < total / 2.5; i++) {
        ++progress_bar3; // record the tick

        usleep(200); // simulate work

        // display the bar only when there was at least 30% progress
        // since the last time it was displayed
        progress_bar3.display();
    }

    //let the bar fail
    progress_bar3.failure("unkown error");

	
    progresscpp::ProgressBar progress_bar4(total, 70);
    for(int i = 0; i < total; i++) {
        ++progress_bar4; // record the tick

        usleep(400); // simulate work

        // display the bar only when there was at least 1% progress
        // since the last time it was displayed
        progress_bar4.displayIfChangedAtLeast(0.1);
    }

    progress_bar4.done();
}
