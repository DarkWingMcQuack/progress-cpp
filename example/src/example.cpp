#include "progresscpp/ProgressBar.hpp"
#include <iostream>
#include <unistd.h>

/* Example usage of ProgressBar */
int main()
{
    const int total = 10000;

    /*
     * Define a progress bar that has a total of 10000,
     * a width of 70, shows `#` to indicate completion
     * and a dash '-' for incomplete
     */
    progresscpp::ProgressBar progressBar0(total, 70, '#', '-');

    for(int i = 0; i < total; i++) {
        ++progressBar0; // record the tick

        usleep(200); // simulate work

        // display the bar only at certain steps
        if(i % 10 == 0)
            progressBar0.display();
    }

    // tell the bar to finish
    progressBar0.done();

    progresscpp::ProgressBar progressBar1(total, 70, '#', '-');

    for(int i = 0; i < total / 2; i++) {
        ++progressBar1; // record the tick

        usleep(200); // simulate work

        // display the bar only at certain steps
        if(i % 10 == 0)
            progressBar1.display();
    }

    progressBar1.failure("unexpected error");
}
