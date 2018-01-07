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
#include <iostream>

using namespace std;
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


static void DrawWeatherLine(FrameCanvas *offscreen, int x0, int y0, int x1, int y1, Color colour, bool centre) {
  int xStart;
  if (centre) {
    xStart = 25;
  } else {
    xStart = 28;
  }

  int yStart = 23;
  DrawLine(offscreen, xStart + x0, yStart + y0, xStart + x1, yStart + y1, colour);
}

static void MakeCloudySymbol(FrameCanvas *offscreen, bool centre) {
  Color white(255, 255, 255);
  Color lightGrey(232, 232, 230);

  //line 1
  DrawWeatherLine(offscreen, 5, 1,  7, 1, lightGrey, centre);
  //line 2
  DrawWeatherLine(offscreen, 4, 2, 4, 2, lightGrey, centre);
  DrawWeatherLine(offscreen, 5, 2, 7, 2, white, centre);
  DrawWeatherLine(offscreen, 8, 2, 8 , 2, lightGrey, centre);
  //line 3
  DrawWeatherLine(offscreen, 4, 3, 4, 3, lightGrey, centre);
  DrawWeatherLine(offscreen, 5, 3, 8, 3, white, centre);
  DrawWeatherLine(offscreen, 9, 3, 9 , 3, lightGrey, centre);
  //line 4
  DrawWeatherLine(offscreen, 2, 4, 3, 4, lightGrey, centre);
  DrawWeatherLine(offscreen, 4, 4, 9, 4, white, centre);
  DrawWeatherLine(offscreen, 10, 4, 11 , 4, lightGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 1, 5, 1, 5, lightGrey, centre);
  DrawWeatherLine(offscreen, 2, 5, 11, 5, white, centre);
  DrawWeatherLine(offscreen, 12, 5, 12 , 5, lightGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 1, 6, 1, 6, lightGrey, centre);
  DrawWeatherLine(offscreen, 2, 6, 11, 6, white, centre);
  DrawWeatherLine(offscreen, 12, 6, 12 , 6, lightGrey, centre);
  //line 7
  DrawWeatherLine(offscreen, 2, 7, 4, 7, lightGrey, centre);
  DrawWeatherLine(offscreen, 5, 7, 9, 7, white, centre);
  DrawWeatherLine(offscreen, 10, 7, 11 , 7, lightGrey, centre);
  //line 8
  DrawWeatherLine(offscreen, 5, 8, 9, 8, lightGrey, centre);
}

static void MakeSunnySymbol(FrameCanvas *offscreen, bool centre) {
  Color orange(240, 198, 0);

  //line 1
  DrawWeatherLine(offscreen, 5, 1, 5, 1, orange, centre);
  DrawWeatherLine(offscreen, 8, 1, 8, 1, orange, centre);
  //line 2 blank
  //line 3
  DrawWeatherLine(offscreen, 3, 3, 3, 3, orange, centre);
  DrawWeatherLine(offscreen, 5, 3, 8, 3, orange, centre);
  DrawWeatherLine(offscreen, 10, 3, 10, 3, orange, centre);
  //line 4
  DrawWeatherLine(offscreen, 5, 4, 8, 4, orange, centre);

  //line 5
  DrawWeatherLine(offscreen, 5, 5, 8, 5, orange, centre);
  //line 6
  DrawWeatherLine(offscreen, 3, 6, 3, 6, orange, centre);
  DrawWeatherLine(offscreen, 5, 6, 8, 6, orange, centre);
  DrawWeatherLine(offscreen, 10, 6, 10, 6, orange, centre);
  //line 7 blank
  //line 8
  DrawWeatherLine(offscreen, 5, 8, 5, 8, orange, centre);
  DrawWeatherLine(offscreen, 8, 8, 8, 8, orange, centre);
}

static void MakePartlyCloudySymbol(FrameCanvas *offscreen, bool centre) {
  Color orange(240, 198, 0);
  Color white(200, 200, 200);
  Color lightGrey(150, 150, 150);
  //line 1
  DrawWeatherLine(offscreen, 5, 1, 5, 1, orange, centre);
  DrawWeatherLine(offscreen, 8, 1, 8, 1, orange, centre);
  //line 2 blank
  //line 3
  DrawWeatherLine(offscreen, 3, 3, 3, 3, orange, centre);
  DrawWeatherLine(offscreen, 5, 3, 8, 3, orange, centre);
  DrawWeatherLine(offscreen, 10, 3, 10, 3, orange, centre);
  //line 4
  DrawWeatherLine(offscreen, 5, 4, 7, 4, orange, centre);
  DrawWeatherLine(offscreen, 8, 4, 9, 4, lightGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 4, 5, 5, 5, lightGrey, centre);
  DrawWeatherLine(offscreen, 6, 5, 6, 5, orange, centre);
  DrawWeatherLine(offscreen, 7, 5, 7, 5, lightGrey, centre);
  DrawWeatherLine(offscreen, 8, 5, 9, 5, white, centre);
  DrawWeatherLine(offscreen, 10, 5, 10, 5, lightGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 3, 6, 3, 6, lightGrey, centre);
  DrawWeatherLine(offscreen, 4, 6, 5, 6, white, centre);
  DrawWeatherLine(offscreen, 6, 6, 6, 6, orange, centre);
  DrawWeatherLine(offscreen, 7, 6, 9, 6, white, centre);
  DrawWeatherLine(offscreen, 10, 6, 10, 6, lightGrey, centre);
  //line 7
  DrawWeatherLine(offscreen, 3, 7, 3, 7, lightGrey, centre);
  DrawWeatherLine(offscreen, 4, 7, 9, 7, white, centre);
  DrawWeatherLine(offscreen, 10, 7, 10, 7, lightGrey, centre);
  //line 8
  DrawWeatherLine(offscreen, 4, 8, 9, 8, lightGrey, centre);
}

static void MakeLightRainSymbol(FrameCanvas *offscreen, bool centre) {
  Color blue(0, 0, 255);
  Color white(255, 255, 255);
  Color lightGrey(232, 232, 230);
  //line 1
  DrawWeatherLine(offscreen, 5, 1, 9, 1, lightGrey, centre);
  //line 2 blank
  DrawWeatherLine(offscreen, 4, 2, 4, 2, lightGrey, centre);
  DrawWeatherLine(offscreen, 5, 2, 9, 2, white, centre);
  DrawWeatherLine(offscreen, 10, 2, 10, 2, lightGrey, centre);
  //line 3
  DrawWeatherLine(offscreen, 3, 3, 3, 3, lightGrey, centre);
  DrawWeatherLine(offscreen, 4, 3, 10, 3, white, centre);
  DrawWeatherLine(offscreen, 11, 3, 11, 3, lightGrey, centre);
  //line 4
  DrawWeatherLine(offscreen, 3, 4, 3, 4, lightGrey, centre);
  DrawWeatherLine(offscreen, 4, 4, 9, 4, white, centre);
  DrawWeatherLine(offscreen, 10, 4, 10, 4, lightGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 4, 5, 9, 5, lightGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 5, 6, 5, 6, blue, centre);
  DrawWeatherLine(offscreen, 8, 6, 8, 6, blue, centre);
  //line 7 blank
  //line 8
  DrawWeatherLine(offscreen, 6, 8, 6, 8, blue, centre);
  DrawWeatherLine(offscreen, 9, 8, 9, 8, blue, centre);
}

static void MakeLightSnowSymbol(FrameCanvas *offscreen, bool centre) {
  Color darkGrey(90, 90, 90);
  Color white(255, 255, 255);
  //line 1
  DrawWeatherLine(offscreen, 5, 1, 9, 1, darkGrey, centre);
  //line 2 blank
  DrawWeatherLine(offscreen, 4, 2, 10, 2, darkGrey, centre);
  //line 3
  DrawWeatherLine(offscreen, 3, 3, 11, 3, darkGrey, centre);
  //line 4
  DrawWeatherLine(offscreen, 3, 4, 10, 4, darkGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 4, 5, 9, 5, darkGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 5, 6, 5, 6, white, centre);
  DrawWeatherLine(offscreen, 8, 6, 8, 6, white, centre);
  //line 7 blank
  //line 8
  DrawWeatherLine(offscreen, 6, 8, 6, 8, white, centre);
  DrawWeatherLine(offscreen, 9, 8, 9, 8, white, centre);
}

static void MakeHeavySnowSymbol(FrameCanvas *offscreen, bool centre) {
  Color darkGrey(90, 90, 90);
  Color white(255, 255, 255);
  //line 1
  DrawWeatherLine(offscreen, 5, 1, 9, 1, darkGrey, centre);
  //line 2 blank
  DrawWeatherLine(offscreen, 4, 2, 11, 2, darkGrey, centre);
  //line 3
  DrawWeatherLine(offscreen, 3, 3, 12, 3, darkGrey, centre);
  //line 4
  DrawWeatherLine(offscreen, 2, 4, 12, 4, darkGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 3, 5, 11, 5, darkGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 4, 6, 4, 6, white, centre);
  DrawWeatherLine(offscreen, 7, 6, 7, 6, white, centre);
  DrawWeatherLine(offscreen, 10, 6, 10, 6, white, centre);
  //line 7
  DrawWeatherLine(offscreen, 5, 7, 5, 7, white, centre);
  DrawWeatherLine(offscreen, 8, 7, 8, 7, white, centre);
  DrawWeatherLine(offscreen, 11, 7, 11, 7, white, centre);
  //line 8
  DrawWeatherLine(offscreen, 6, 8, 6, 8, white, centre);
  DrawWeatherLine(offscreen, 9, 8, 9, 8, white, centre);
  DrawWeatherLine(offscreen, 12, 8, 12, 8, white, centre);
}

static void MakeHeavyRainSymbol(FrameCanvas *offscreen, bool centre) {
  Color darkGrey(90, 90, 90);
  Color blue(0, 0, 255);
  //line 1
  DrawWeatherLine(offscreen, 5, 1, 9, 1, darkGrey, centre);
  //line 2 blank
  DrawWeatherLine(offscreen, 4, 2, 11, 2, darkGrey, centre);
  //line 3
  DrawWeatherLine(offscreen, 3, 3, 12, 3, darkGrey, centre);
  //line 4
  DrawWeatherLine(offscreen, 2, 4, 12, 4, darkGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 3, 5, 11, 5, darkGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 4, 6, 4, 6, blue, centre);
  DrawWeatherLine(offscreen, 7, 6, 7, 6, blue, centre);
  DrawWeatherLine(offscreen, 10, 6, 10, 6, blue, centre);
  //line 7
  DrawWeatherLine(offscreen, 5, 7, 5, 7, blue, centre);
  DrawWeatherLine(offscreen, 8, 7, 8, 7, blue, centre);
  DrawWeatherLine(offscreen, 11, 7, 11, 7, blue, centre);
  //line 8
  DrawWeatherLine(offscreen, 6, 8, 6, 8, blue, centre);
  DrawWeatherLine(offscreen, 9, 8, 9, 8, blue, centre);
  DrawWeatherLine(offscreen, 12, 8, 12, 8, blue, centre);
}

static void MakeSleetSymbol(FrameCanvas *offscreen, bool centre) {
  Color darkGrey(90, 90, 90);
  Color blue(0, 0, 255);
  Color white(255, 255, 255);
  //line 1
  DrawWeatherLine(offscreen, 5, 1, 9, 1, darkGrey, centre);
  //line 2 blank
  DrawWeatherLine(offscreen, 4, 2, 11, 2, darkGrey, centre);
  //line 3
  DrawWeatherLine(offscreen, 3, 3, 12, 3, darkGrey, centre);
  //line 4
  DrawWeatherLine(offscreen, 2, 4, 12, 4, darkGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 3, 5, 11, 5, darkGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 4, 6, 4, 6, blue, centre);
  DrawWeatherLine(offscreen, 7, 6, 7, 6, white, centre);
  DrawWeatherLine(offscreen, 10, 6, 10, 6, blue, centre);
  //line 7
  DrawWeatherLine(offscreen, 5, 7, 5, 7, white, centre);
  DrawWeatherLine(offscreen, 8, 7, 8, 7, blue, centre);
  DrawWeatherLine(offscreen, 11, 7, 11, 7, white, centre);
  //line 8
  DrawWeatherLine(offscreen, 6, 8, 6, 8, blue, centre);
  DrawWeatherLine(offscreen, 9, 8, 9, 8, white, centre);
  DrawWeatherLine(offscreen, 12, 8, 12, 8, blue, centre);
}

static void MakeThunderSymbol(FrameCanvas *offscreen, bool centre) {
  Color darkGrey(90, 90, 90);
  Color blue(0, 0, 255);
  Color yellow(255, 240, 0);
  //line 1
  DrawWeatherLine(offscreen, 6, 1, 10, 1, darkGrey, centre);
  //line 2 blank
  DrawWeatherLine(offscreen, 5, 2, 12, 2, darkGrey, centre);
  //line 3
  DrawWeatherLine(offscreen, 4, 3, 13, 3, darkGrey, centre);
  //line 4
  DrawWeatherLine(offscreen, 3, 4, 13, 4, darkGrey, centre);
  //line 5
  DrawWeatherLine(offscreen, 2, 5, 12, 5, darkGrey, centre);
  //line 6
  DrawWeatherLine(offscreen, 4, 6, 4, 6, blue, centre);
  DrawWeatherLine(offscreen, 7, 6, 8, 6, yellow, centre);
  DrawWeatherLine(offscreen, 11, 6, 11, 6, blue, centre);
  //line 7
  DrawWeatherLine(offscreen, 3, 7, 3, 7, blue, centre);
  DrawWeatherLine(offscreen, 6, 7, 7, 7, yellow, centre);
  DrawWeatherLine(offscreen, 10, 7, 10, 7, blue, centre);
  //line 8
  DrawWeatherLine(offscreen, 2, 8, 2, 8, blue, centre);
  DrawWeatherLine(offscreen, 6, 8, 6, 8, yellow, centre);
  DrawWeatherLine(offscreen, 9, 8, 9, 8, blue, centre);
}

static void WriteText(RGBMatrix * canvas) {
  FrameCanvas *offscreen = canvas->CreateFrameCanvas();
  const char *bdf_font_file = "../fonts/4x6.bdf";
  const char *bdf_big_font_file = "../fonts/clock.bdf";
  const char *bdf_times_font_file = "../fonts/times.bdf";
  Font font;
  font.LoadFont(bdf_font_file);
  Font bigFont;
  bigFont.LoadFont(bdf_big_font_file);
  Font timesFont;
  timesFont.LoadFont(bdf_times_font_file);
  struct timespec next_time;
  struct tm tm;
  next_time.tv_sec = time(NULL);
  next_time.tv_nsec = 0;

  while (!interrupt_received) {
    try{
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
      Color yellow(255, 100, 0);
      Color green(0, 255, 0);
      Color white(255, 255, 255);
      Color coldBlue(0, 228, 255);

      localtime_r(&next_time.tv_sec, &tm);
      char time_buffer[256];
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
      DrawText(offscreen, bigFont, 15, 32,
        red, NULL, highTemp.c_str(), 0);

      //weather
      bool shouldCentre = highTemp.length() < 4;

      MakePartlyCloudySymbol(offscreen, shouldCentre);

      clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next_time, NULL);

      // Atomic swap with double buffer
      offscreen = canvas->SwapOnVSync(offscreen);


    } catch(const std::exception& e) {
      printf("An error occured");
    }
    next_time.tv_sec = time(NULL) + 1;
  }
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
  defaults.rows = 32;
  defaults.chain_length = 2;
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
