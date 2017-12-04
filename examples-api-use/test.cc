// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fstream>
#include <string>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
using rgb_matrix::Font;
using rgb_matrix::Color;
using rgb_matrix::DrawText;
using rgb_matrix::FrameCanvas;

#include "led-matrix.h"
#include "graphics.h"

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

static void DrawOnCanvas(Canvas *canvas) {
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   */
  // canvas->Fill(0, 0, 255);

  while (!interrupt_received) {
    int center_x = canvas->width() / 2;
    int center_y = canvas->height() / 2;
    float radius_max = canvas->width() / 2;
    float angle_step = 1.0 / 360;
    for (float a = 0; a < 255; a += 1) {
      if (interrupt_received)
        return;
        canvas->Fill(a, 0, a);
      // canvas->SetPixel(center_x, center_y,
      //                  a, a, a);
      usleep(1 * 10000);  // wait a little to slow down things.
    }
  }

}

static void WriteText(RGBMatrix * canvas) {
  FrameCanvas *offscreen = canvas->CreateFrameCanvas();
  while (!interrupt_received) {
    const char *bdf_font_file = "../fonts/4x6.bdf";
    const char *bdf_big_font_file = "../fonts/clock.bdf";
    const char *bdf_times_font_file = "../fonts/times.bdf";
    Font font;
    font.LoadFont(bdf_font_file);
    Font bigFont;
    bigFont.LoadFont(bdf_big_font_file);
    Font timesFont;
    timesFont.LoadFont(bdf_times_font_file);

    std::ifstream trainTimesFile("trainTimes.txt");

    std::string red1, green1, blue1, time1, dest1;
    std::getline(trainTimesFile, red1);
    std::getline(trainTimesFile, green1);
    std::getline(trainTimesFile, blue1);
    std::getline(trainTimesFile, time1);
    std::getline(trainTimesFile, dest1);
    Color color1(std::stoi(red1), std::stoi(green1), std::stoi(blue1));

    std::string red2, green2, blue2, time2, dest2;
    std::getline(trainTimesFile, red2);
    std::getline(trainTimesFile, green2);
    std::getline(trainTimesFile, blue2);
    std::getline(trainTimesFile, time2);
    std::getline(trainTimesFile, dest2);
    Color color2(std::stoi(red2), std::stoi(green2), std::stoi(blue2));

    std::string red3, green3, blue3, time3, dest3;
    std::getline(trainTimesFile, red3);
    std::getline(trainTimesFile, green3);
    std::getline(trainTimesFile, blue3);
    std::getline(trainTimesFile, time3);
    std::getline(trainTimesFile, dest3);
    Color color3(std::stoi(red3), std::stoi(green3), std::stoi(blue3));

    std::ifstream weatherFile("weather.txt");
    std::string lowTemp, highTemp;
    std::getline(weatherFile, lowTemp);
    std::getline(weatherFile, highTemp);

    offscreen->Fill(0, 0, 0);

    Color red(255, 0, 0);
    Color yellow(255, 255, 0);
    Color green(0, 255, 0);
    Color white(255, 255, 255);
    Color coldBlue(0, 228, 255);
    char time_buffer[256];
    struct timespec next_time;
    next_time.tv_sec = time(NULL);
    next_time.tv_nsec = 0;
    struct tm tm;
    localtime_r(&next_time.tv_sec, &tm);
    strftime(time_buffer, sizeof(time_buffer), "%H:%M", &tm);
    //1st destination
    DrawText(offscreen, timesFont, 45, 5,
                         color1, NULL, time1.c_str(), 0);
    DrawText(offscreen, font, 0, 5,
                         yellow, NULL, dest1.c_str(), 0);

    //2nd destination
    DrawText(offscreen, timesFont, 45, 12,
                         color2, NULL, time2.c_str(), 0);
    DrawText(offscreen, font, 0, 12,
                         yellow, NULL, dest2.c_str(), 0);
    //3rd destination
    DrawText(offscreen, timesFont, 45, 19,
                         color3, NULL, time3.c_str(), 0);
    DrawText(offscreen, font, 0, 19,
                         yellow, NULL, dest3.c_str(), 0);
    //clock
    DrawText(offscreen, bigFont, 43, 32,
                         white, NULL, time_buffer, 0);
    //temperatures
    DrawText(offscreen, bigFont, 0, 32,
                         coldBlue, NULL, lowTemp.c_str(), 0);
    DrawText(offscreen, bigFont, 14, 32,
                         red, NULL, highTemp.c_str(), 0);
    // DrawText(offscreen, bigFont, 0, 31,
    //                      white, NULL, "1234567890", -1);

     // Atomic swap with double buffer
     offscreen = canvas->SwapOnVSync(offscreen);

     next_time.tv_sec += 1;
    // canvas->Clear();
  }
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  defaults.rows = 32;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;

  RGBMatrix *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);

  if (canvas == NULL)
    return 1;
  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  // DrawOnCanvas(canvas);    // Using the canvas.
  WriteText(canvas);

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
























//end
