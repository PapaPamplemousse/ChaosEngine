#include "chaos_engine.h"

t_sint32 main() {
    // Physics setup
    t_uint8 dim = 3;
    world_t world;
    // Initializing world with 3D gravity: vec3(0, -9.81, 0)
    world_init(&world, vec3(0.0, -9.81, 0.0));

    body_t ball;
    // Initializing ball with mass=1.0, position=(0, 5, 0), dimension=3
    body_init(&ball, 1.0, vec3(0.0, 5.0, 0.0), dim);

    // Renderer setup
    renderer_t renderer;
    // Window title uses t_char, width/height use t_sint32
    if (renderer_init(&renderer, "ChaosEngine Demo", 800, 600) != 0)
        return -1;

    // Camera setup
    camera_set_position(&renderer, vec3(5.0, 5.0, 10.0));
    camera_look_at(&renderer, vec3(0.0, 0.0, 0.0));

    // Primitive setup
    primitive_t pball = {0}, ground = {0};
    
    // Ball Primitive
    pball.type = PRIMITIVE_SPHERE;
    pball.position = ball.pos;
    pball.scale = vec3(1.0, 1.0, 1.0);
    // Color components use t_uint8 (0-255)
    pball.color[0] = 255; pball.color[1] = 50; pball.color[2] = 50;

    // Ground Primitive
    ground.type = PRIMITIVE_PLANE;
    ground.position = vec3(0.0, 0.0, 0.0);
    ground.scale = vec3(20.0, 1.0, 20.0);
    ground.color[0] = 0; ground.color[1] = 200; ground.color[2] = 0;

    // Array of objects to draw
    primitive_t* objects[2] = { &pball, &ground };

    // Game loop variables
    t_float64 dt = 0.016; // Delta time (t_float64)
    SDL_Event e;
    t_sint32 running = 1; // Running flag (t_sint32)

    while (running) {
        // Event loop
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
        }

        // Physics step
        world_step(&world, &ball, dt);
        
        // Simple collision (using t_float64 for comparison)
        if (ball.pos.y < 0.0) {
            ball.pos.y = 0.0;
            // Velocity components are t_float64
            ball.vel.y *= -0.7; 
        }

        // Update graphics position from physics
        pball.position = ball.pos;

        // Rendering
        renderer_clear(&renderer);
        // Count uses t_sint32
        draw_scene(&renderer, objects, 2); 
        renderer_present(&renderer);

        // Frame timing (SDL_Delay expects milliseconds, result cast to t_sint32)
        SDL_Delay((t_sint32)(dt * 1000.0));
    }

    // Cleanup
    renderer_destroy(&renderer);
    return 0; // Returns t_sint32
}