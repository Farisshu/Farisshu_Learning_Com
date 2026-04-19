/**
 * @file main.cpp
 * @brief Starfield Lyric Animation Display
 * @version 1.0
 * @date 2024
 * 
 * This module displays animated starfield background with song lyrics
 * from "Love Me Not" by Ravyn Lenae on a TFT display.
 * Features 3D star projection and smooth fade effects.
 * 
 * @copyright Copyright (c) 2024
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

/* === Display Configuration (Landscape) === */
#define DISPLAY_WIDTH       160                 /**< Display width in pixels */
#define DISPLAY_HEIGHT      128                 /**< Display height in pixels */
#define CENTER_X            (DISPLAY_WIDTH / 2) /**< Center X coordinate */
#define CENTER_Y            (DISPLAY_HEIGHT / 2)/**< Center Y coordinate */

/* === Frame Rate Control (30 FPS) === */
#define TARGET_FPS          30                  /**< Target frames per second */
#define FRAME_TIME_MS       (1000 / TARGET_FPS) /**< Frame duration in ms */

/* === Starfield Configuration === */
#define NUM_STARS           50                  /**< Number of stars */
#define STAR_Z_MIN          1                   /**< Minimum depth */
#define STAR_Z_MAX          350                 /**< Maximum depth */
#define STAR_SPEED_MIN      0.5f                /**< Minimum star speed */
#define STAR_SPEED_MAX      1.5f                /**< Maximum star speed */

/* === Lyric Timing === */
#define FADE_IN_MS          300                 /**< Fade in duration */
#define LYRIC_DISPLAY_MS    2000                /**< Full visibility duration */
#define FADE_OUT_MS         500                 /**< Fade out duration */
#define LINE_TOTAL_MS       (FADE_IN_MS + LYRIC_DISPLAY_MS + FADE_OUT_MS)

/* === Color Definitions === */
#define COLOR_BG            0x0000              /**< Black background */
#define COLOR_TEXT          0xFFFF              /**< White text */
#define COLOR_STAR_DIM      0x0A50              /**< Dark blue for distant stars */
#define COLOR_TEXT_BG       0x000F              /**< Dark blue for text background */

/* === Hardware Instance === */
static TFT_eSPI tft = TFT_eSPI();               /**< TFT display instance */

/* === Star Structure === */
typedef struct {
    float x;                                    /**< X position (-width to width) */
    float y;                                    /**< Y position (-height to height) */
    float z;                                    /**< Depth (Z-axis) */
    float speed;                                /**< Movement speed */
} Star_t;

/* === Global State === */
static Star_t stars[NUM_STARS];                 /**< Star array */
static int currentLineIndex = 0;                /**< Current lyric line index */
static unsigned long lineStartTime = 0;         /**< Start time of current line */
static bool animationFinished = false;          /**< Animation completion flag */
static unsigned long lastFrameTime = 0;         /**< Last frame timestamp */

/* === Song Lyrics (Love Me Not - Ravyn Lenae) === */
static const char* lyrics[] = {
    "And, oh,",
    "it's hard to see you,",
    "but I wish you were",
    "right here.",
    "", 
    "Oh, it's hard to leave you",
    "when I get you",
    "everywhere."
};
static const int TOTAL_LYRIC_LINES = 8;         /**< Total number of lyric lines */

/**
 * @brief Initialize stars with random positions and speeds
 */
static void initStars(void) {
    for (int i = 0; i < NUM_STARS; i++) {
        stars[i].x = random(-DISPLAY_WIDTH, DISPLAY_WIDTH);
        stars[i].y = random(-DISPLAY_HEIGHT, DISPLAY_HEIGHT);
        stars[i].z = random(STAR_Z_MIN + 19, STAR_Z_MAX);
        stars[i].speed = map(random(5, 15), 5, 15, STAR_SPEED_MIN, STAR_SPEED_MAX);
    }
}

/**
 * @brief Draw starfield background with 3D projection effect
 */
static void drawStarfield(void) {
    for (int i = 0; i < NUM_STARS; i++) {
        /* Move star toward viewer */
        stars[i].z -= stars[i].speed;
        
        /* Reset star if it passes the screen */
        if (stars[i].z <= STAR_Z_MIN) {
            stars[i].x = random(-DISPLAY_WIDTH, DISPLAY_WIDTH);
            stars[i].y = random(-DISPLAY_HEIGHT, DISPLAY_HEIGHT);
            stars[i].z = STAR_Z_MAX;
            stars[i].speed = map(random(5, 15), 5, 15, STAR_SPEED_MIN, STAR_SPEED_MAX);
            continue;
        }

        /* Project 3D coordinates to 2D screen */
        const float screenX = (stars[i].x / stars[i].z) * CENTER_X + CENTER_X;
        const float screenY = (stars[i].y / stars[i].z) * CENTER_Y + CENTER_Y;
        const int starSize = map(stars[i].z, STAR_Z_MIN, STAR_Z_MAX, 2, 1);
        
        /* Calculate brightness based on distance */
        const uint16_t starColor = map(stars[i].z, STAR_Z_MIN, STAR_Z_MAX, COLOR_TEXT, COLOR_STAR_DIM);
        
        /* Draw star if within screen bounds */
        if (screenX >= 0 && screenX <= DISPLAY_WIDTH && screenY >= 0 && screenY <= DISPLAY_HEIGHT) {
            tft.fillCircle(screenX, screenY, starSize, starColor);
        }
    }
}

/**
 * @brief Draw current lyric line with fade in/out effect
 */
static void drawLyrics(void) {
    const unsigned long elapsed = millis() - lineStartTime;
    uint8_t alpha = 0;
    
    /* Calculate alpha based on timing phase */
    if (elapsed < FADE_IN_MS) {
        /* Fade IN phase */
        alpha = map(elapsed, 0, FADE_IN_MS, 0, 255);
    } else if (elapsed < (FADE_IN_MS + LYRIC_DISPLAY_MS)) {
        /* Fully visible phase */
        alpha = 255;
    } else if (elapsed < LINE_TOTAL_MS) {
        /* Fade OUT phase */
        alpha = map(elapsed, FADE_IN_MS + LYRIC_DISPLAY_MS, LINE_TOTAL_MS, 255, 0);
    } else {
        /* Time to advance to next line */
        currentLineIndex++;
        lineStartTime = millis();
        alpha = 0;
        
        if (currentLineIndex >= TOTAL_LYRIC_LINES) {
            animationFinished = true;
        }
    }
    
    /* Draw lyrics if animation not finished */
    if (currentLineIndex < TOTAL_LYRIC_LINES) {
        /* Clear text area with semi-transparent background */
        tft.fillRect(10, CENTER_Y - 20, DISPLAY_WIDTH - 20, 40, COLOR_TEXT_BG);
        
        /* Calculate RGB color with alpha blending */
        const uint8_t intensity = alpha;
        const uint16_t textColor = tft.color565(intensity, intensity, intensity);

        /* Draw centered text */
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(textColor, COLOR_TEXT_BG);
        tft.setTextSize(1);
        tft.drawString(lyrics[currentLineIndex], CENTER_X, CENTER_Y);
    }
}

/**
 * @brief Initialize display and start animation
 */
void setup() {
    Serial.begin(115200);
    
    /* Initialize TFT display */
    tft.init();
    tft.setRotation(1);                       /**< Landscape orientation */
    tft.fillScreen(COLOR_BG);
    
    /* Initialize animation state */
    initStars();
    lineStartTime = millis();
    lastFrameTime = millis();
    
    Serial.println("▶ Starfield Lyric Mode Ready");
}

/**
 * @brief Main animation loop with frame rate control
 */
void loop() {
    if (animationFinished) {
        return;                               /**< Stop animation when done */
    }
    
    /* Frame rate control */
    const unsigned long currentTime = millis();
    const unsigned long frameDelta = currentTime - lastFrameTime;
    
    if (frameDelta < FRAME_TIME_MS) {
        delay(FRAME_TIME_MS - frameDelta);
        return;
    }
    lastFrameTime = currentTime;
    
    /* Render complete frame using SPI batch operations */
    tft.startWrite();
    
    /* Step 1: Draw trailing effect for motion blur */
    tft.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, tft.color565(0, 0, 5));
    
    /* Step 2: Draw starfield */
    drawStarfield();
    
    /* Step 3: Draw lyrics on top */
    drawLyrics();
    
    /* Draw final heart if animation complete */
    if (animationFinished) {
        tft.fillRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, tft.color565(0, 0, 10));
        tft.setTextColor(COLOR_TEXT, tft.color565(0, 0, 10));
        tft.setTextSize(3);
        tft.drawString("♥", CENTER_X, CENTER_Y);
    }

    tft.endWrite();
}
