#include <stdio.h>
#include <assert.h>

#include <emscripten.h>

int mains = 0, inners = 0, nevers, loops = 0;

void loop(void) {
  emscripten_sleep(1);
  loops++;
  printf("loop:  mains, inners, nevers, loops: %d, %d, %d, %d\n", mains, inners, nevers, loops);
  if (loops > 5) {
    printf("done looping\n");
    emscripten_cancel_main_loop();
    assert(mains == 1); // never re-enter main
    assert(inners == 1); // never re-enter inner
    assert(nevers == 0); // never reach never
    int result = 1;
    REPORT_RESULT();
    return;
  }
}

void inner() {
  inners++;
  printf("inner: mains, inners, nevers, loops: %d, %d, %d, %d\n", mains, inners, nevers, loops);
  emscripten_set_main_loop(loop, 1, 1);
  nevers++;
}

int main(void) {
  mains++;
  printf("main:  mains, inners, nevers, loops: %d, %d, %d, %d\n", mains, inners, nevers, loops);
  inner();
}

