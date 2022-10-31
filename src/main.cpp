#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <bits/stdc++.h>

void ImageDrawFilledCircleV(Image *image, const Vector2 pos, float radius, const Color color) {
    for( int i = 0; i < radius; ++i ) {
        ImageDrawCircleV(image, pos, i, color);
    }
}


int main(int argc, char** argv)
{
    constexpr static int width = 1000;
    constexpr static int height = 1000;
    constexpr static int panel_start = width;
    constexpr static int panel_size = 400;
    constexpr static int spacing = 10;
    constexpr static int hspacing = 30;

    InitWindow(width + panel_size, height, "smoketest");

    auto get_points = [&width](int count){
        std::vector<Vector2> res;
        res.reserve(count);

        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, width);

        for( auto i : std::ranges::views::iota(0, count) ) {
            res.emplace_back(dist(gen), dist(gen));
        }

        return res;
    };

    int points_count = 10;
    std::vector<Vector2> points = get_points(points_count);
    bool draw_points = true;
    char pic_name[255];
    Image pfp = GenImageColor(width, height, BLACK);

    float points_radius = 10.f;
    Color points_color = BLACK;
    Color start_color = GREEN;
    Color end_color = WHITE;
    float start_distance = 0.f;
    float end_distance_multiplier = 1.f;

    auto regen_image = [&](){
        ImageClearBackground(&pfp, BLACK);

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                std::vector<float> distances;
                distances.reserve(points_count);

                for (auto& point : points)
                    distances.emplace_back(Vector2Distance(point, {x,y}));

                std::sort(distances.begin(), distances.end());

                float ammout = Remap(distances[0]+start_distance, 0, end_distance_multiplier * distances.back(), 0, 1);

                Color pix_color{Lerp(start_color.r, end_color.r, ammout), Lerp(start_color.g, end_color.g, ammout), Lerp(start_color.b, end_color.b, ammout), 255};

                ImageDrawPixel(&pfp, x, y, pix_color);
            }
        }
        if(draw_points) {
            for(auto& point : points) {
                ImageDrawFilledCircleV(&pfp, point, points_radius, points_color);
            }
        }
    };

    regen_image();
    Texture2D tx = LoadTextureFromImage(pfp);


    while (!WindowShouldClose())
    {
        BeginDrawing();


            ClearBackground(BLACK);
            DrawTexture(tx, 0, 0, WHITE);

            GuiPanel({panel_start, 0, panel_size, height}, "Controls");
            int offset = spacing + 30;
            if( GuiButton({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Regenerate image")) {
                regen_image();
                UnloadTexture(tx);
                tx = LoadTextureFromImage(pfp);
            }
            offset += 30 + spacing;


            GuiLabel({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Points count : ");
            offset += 30;
            points_count = GuiSliderBar({panel_start + hspacing, offset, panel_size - 2*hspacing, 30}, "0", "100", points_count, 0, 100);
            if( points.size() != points_count ) {
                points = get_points(points_count);
            }
            offset += 30 + spacing;
            draw_points = GuiToggle({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Draw points", draw_points);
            offset += 30 + spacing;
            if( GuiButton({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Regenerate points")) {
                points = get_points(points_count);
                regen_image();
                UnloadTexture(tx);
                tx = LoadTextureFromImage(pfp);
            }
            offset += 30 + spacing;


            GuiLabel({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Picture name: ");
            offset += 30;
            GuiTextBox({panel_start + spacing, offset, panel_size - 2*spacing, 30}, pic_name, 255, true);
            offset += 30 + spacing;
            if( GuiButton({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Export Image image")) {
                ExportImage(pfp, pic_name);
            }
            offset += 30 + spacing;


            GuiLabel({panel_start + hspacing, offset, panel_size - 2*spacing, 30}, "Points radius: ");
            offset += 30;
            points_radius = GuiSliderBar({panel_start + hspacing, offset, panel_size - 2*hspacing, 30}, "10", "100", points_radius, 10, 100);
            offset += 30 + spacing;

            GuiLabel({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Points color");
            offset += 30;
            points_color = GuiColorPicker({panel_start + hspacing, offset, panel_size - 2*hspacing, 30}, "Points color", points_color);
            offset += 30 + spacing;
            GuiLabel({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Start color");
            offset += 30;
            start_color = GuiColorPicker({panel_start + hspacing, offset, panel_size - 2*hspacing, 30}, "Start color", start_color);
            offset += 30 + spacing;
            GuiLabel({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "End color");
            offset += 30;
            end_color = GuiColorPicker({panel_start + hspacing, offset, panel_size - 2*hspacing, 30}, "End color", end_color);
            offset += 30 + spacing;


            GuiLabel({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Distance start : ");
            offset += 30;
            start_distance = GuiSliderBar({panel_start + hspacing, offset, panel_size - 2*hspacing, 30}, "-100", "100", start_distance, -100, 100);
            offset += 30 + spacing;
            GuiLabel({panel_start + spacing, offset, panel_size - 2*spacing, 30}, "Distance multiplier : ");
            offset += 30;
            end_distance_multiplier = GuiSliderBar({panel_start + hspacing, offset, panel_size - 2*hspacing, 30}, "0", "1", end_distance_multiplier, 0, 1);
            offset += 30 + spacing;
        EndDrawing();
    }

    CloseWindow();
    return 0;
}