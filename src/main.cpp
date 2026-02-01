#include "./../../raylib/src/raylib.h"
#include "images.cpp"
#include "sounds.cpp"
static const int TEXTURE_COUNT = 2;
static const int POINT_COUNT = 8;
static const int SOUND_COUNT = 2;
//
typedef struct {
  int WindowWidth;
  int WindowHeight;
  Texture2D textures[TEXTURE_COUNT];
  Texture2D texture;
  int textureIndex;
  float moveX;
  float moveY;
  float dx;
  float dy;
  Vector2 points[POINT_COUNT];
  int colorChange;
  Sound sounds[SOUND_COUNT];
  bool playSound;

} StateStruct;
int main(void) {
  StateStruct State = {
      .WindowWidth = 800,
      .WindowHeight = 450,
      .textures = {},
      .texture = {},
      .textureIndex = 0,
      .moveX = 100,
      .moveY = 100,
      .dx = 1.0f,
      .dy = 0.7f,
      .points = {},
      .colorChange = 0,
      .sounds = {},
      .playSound = false,
  };
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(State.WindowWidth, State.WindowHeight, "DVD Screen Saver");
  SetTargetFPS(60);
  InitAudioDevice();
  //
  Image image = LoadImageFromMemory(".png", DVD_png, DVD_png_len);
  ImageResize(&image, image.width * 0.5f, image.height * 0.5f);
  Image image2 = LoadImageFromMemory(".png", dietCoke_png, dietCoke_png_len);
  ImageResize(&image2, image2.width * 0.5f, image2.height * 0.5f);
  //
  State.textures[0] = LoadTextureFromImage(image);
  State.textures[1] = LoadTextureFromImage(image2);
  State.texture = State.textures[0];
  //
  SetWindowIcon(image);
  //
  UnloadImage(image);
  UnloadImage(image2);
  //
  Wave wave1 =
      LoadWaveFromMemory(".mp3", __bone_crack_1__mp3, __bone_crack_1__mp3_len);
  Wave wave2 =
      LoadWaveFromMemory(".mp3", __hitmarker_2_mp3, __hitmarker_2_mp3_len);
  State.sounds[0] = LoadSoundFromWave(wave1);
  State.sounds[1] = LoadSoundFromWave(wave2);
  UnloadWave(wave1);
  UnloadWave(wave2);

  while (!WindowShouldClose()) {
    State.WindowWidth = GetScreenWidth();
    State.WindowHeight = GetScreenHeight();
    //
    BeginDrawing();
    ClearBackground(BLACK);
    //
    DrawText(
        "Controls:\nReset: Enter\nChange Texture: T\nSound: S\nHold 'Space' For Collision\n",
        100, 50, 20, LIGHTGRAY);
    if (State.playSound) {
      DrawText("SOUND ON", 100, 175, 20, LIGHTGRAY);
    }
    //
    if (State.textureIndex == 0) {
      switch (State.colorChange) {
      case 0:
        DrawTexture(State.texture, State.moveX, State.moveY, WHITE);
        break;
      case 1:
        DrawTexture(State.texture, State.moveX, State.moveY, MAGENTA);
        break;
      case 2:
        DrawTexture(State.texture, State.moveX, State.moveY, GOLD);
        break;
      case 3:
        DrawTexture(State.texture, State.moveX, State.moveY, SKYBLUE);
        break;
      case 4:
        DrawTexture(State.texture, State.moveX, State.moveY, LIME);
        break;
      }
      State.texture.width = State.WindowWidth * .10;
      State.texture.height = State.WindowHeight * .10;
    } else if (State.textureIndex == 1) {
      DrawTexture(State.texture, State.moveX, State.moveY, WHITE);
      State.texture.width = State.WindowWidth * .10;
      State.texture.height = State.WindowHeight * .20;
    }
    //
    //
    State.moveX += State.dx * 3.0f;
    State.moveY += State.dy * 3.0f;
    State.points[0].x = State.moveX;
    State.points[0].y = State.moveY;
    State.points[1].x = State.moveX + State.texture.width;
    State.points[1].y = State.moveY;
    State.points[2].x = State.moveX;
    State.points[2].y = State.moveY + State.texture.height;
    State.points[3].x = State.moveX + State.texture.width;
    State.points[3].y = State.moveY + State.texture.height;
    State.points[4].x = 0;
    State.points[4].y = 0;
    State.points[5].x = State.WindowWidth;
    State.points[5].y = 0;
    State.points[6].x = 0;
    State.points[6].y = State.WindowHeight;
    State.points[7].x = State.WindowWidth;
    State.points[7].y = State.WindowHeight;
    //
    for (int i = 0; i < 8; i++) {
      if (IsKeyPressedRepeat(KEY_SPACE)) {
        DrawCircle(State.points[i].x, State.points[i].y, 10, RED);
      }
      if (i < 4) {
        //
        if (State.points[i].x <= 0 || State.points[i].x >= State.WindowWidth) {
          State.dx *= -1;
          State.colorChange = GetRandomValue(0, 4);
          break;
        }
        if (State.points[i].y <= 0 || State.points[i].y >= State.WindowHeight) {
          State.dy *= -1;
          State.colorChange = GetRandomValue(0, 4);
          break;
        }
      }
      //
    }
    for (int i = 0; i < 4; i++) {
      for (int j = 4; j < 8; j++) {
        if (CheckCollisionCircles(
                {.x = State.points[i].x, .y = State.points[i].y}, 10,
                {.x = State.points[j].x, .y = State.points[j].y}, 10) &&
            State.playSound) {
          if (!IsSoundPlaying(State.sounds[0]) and
              !IsSoundPlaying(State.sounds[1])) {
            switch (GetRandomValue(0, 1)) {
            case 0:
              PlaySound(State.sounds[0]);
              break;
            case 1:
              PlaySound(State.sounds[1]);
              break;
            }
          }
        }
      }
    }
    if (IsKeyPressed(KEY_ENTER)) {
      State.moveX = (float)GetScreenWidth() / 2;
      State.moveY = (float)GetScreenHeight() / 2;
    }
    if (IsKeyPressed(KEY_T)) {
      State.textureIndex = (State.textureIndex + 1) % TEXTURE_COUNT;
      State.texture = State.textures[State.textureIndex];
    }
    if (IsKeyPressed(KEY_S)) {
      State.playSound = !State.playSound;
    }
    //
    EndDrawing();
  }
  UnloadSound(State.sounds[0]);
  UnloadSound(State.sounds[1]);
  CloseAudioDevice();
  UnloadTexture(State.texture);
  CloseWindow();

  return 0;
}
