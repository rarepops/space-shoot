# Space Shoot

A top-down 2D space shooter: you pilot a lone cruiser sent to clear a sector
of enemy turrets that heavily outnumber you. Speed, firepower, and keeping an
eye on your ship's internal systems are all that stand between you and the
scrapyard. Originally a three-person group project for the Game Programming
course at the IT University of Copenhagen (autumn 2017).

> Built with **C++14** on
> [SimpleRenderEngine](https://github.com/mortennobel/SimpleRenderEngine)
> (SDL2 / OpenGL) with **Box2D** physics and an **ImGui** debug UI.
> Targets Windows x86 via Visual Studio 2017.

## How it works

- **Component-based architecture** — every in-game entity is a `GameObject`
  decorated with `Component`s; the scene is a flat list of game objects that
  the main loop updates and renders each frame.
- **Box2D everywhere** — movement, rotation, projectiles, and hit detection
  all run through the physics world (fixed 1/60 timestep, collision groups
  for player and enemies, contact callbacks routed to components).
- **Ship subsystems** — energy and health are modeled with `Capacitor`
  (a depletable pool) and `Regenerator` (a self-refilling pool that pauses
  after damage); turrets drain the capacitor with every shot.
- **Turrets** — a `TurretController` manages each ship's turret mounts;
  turrets auto-aim (enemies target the player when in range) and support
  spread fire. Bullets are short-lived Box2D bodies.
- **Camera** — a follow camera with custom zoom levels and camera shake.
- **Presentation** — sprites come from a packed sprite atlas rendered through
  SRE sprite batches, with a starfield background, game states with music
  (SDL_mixer), and an ImGui overlay for ship status and debugging.

### Controls

| Input | Action |
|-------|--------|
| `W` / `S` | Thrust forward / backward |
| `A` / `D` | Rotate left / right |
| Mouse + left button | Aim / shoot |
| `Z` | Camera zoom |
| `X` | Toggle debug draw |

## Project layout

```
CMakeLists.txt                builds the game as an SRE subproject
src/
  main.cpp                    entry point
  SpaceShoot.*                game loop, physics world, spawning, game state
  GameObject.* / Component.*  entity/component core
  PhysicsComponent.*          Box2D body wrapper
  Box2DDebugDraw.*            collision-shape debug rendering
  FollowCamera.*              player-tracking camera (zoom, shake)
  SpriteComponent.*           atlas sprite rendering
  ShipComponent.*             hull, damage, and destruction
  Capacitor.* / Regenerator.* energy/health pools
  TurretController.*          per-ship turret mounts and aiming
  TurretComponent.*           individual turret firing logic
  BulletComponent.*           projectile lifetime and collisions
assets/
  spaceshooter.json / .png    sprite atlas
  danger.ogg                  music
docs/
  Space Shooter.docx          original project report
```

## Building

The game builds as a subproject of the SimpleRenderEngine project template,
which bundles all dependencies (SDL2, GLEW, Box2D, ImGui) for Windows:

1. Clone
   [SimpleRenderEngineProject](https://github.com/mortennobel/SimpleRenderEngineProject)
   (this game was developed against SRE 0.9.21, autumn 2017 — newer engine
   versions may need small adjustments).
2. Clone this repository into a `SpaceShoot/` folder at the project root.
3. Append `add_subdirectory(SpaceShoot)` to the root `CMakeLists.txt`.
4. Run CMake and generate for **Visual Studio 2017**, platform **Win32**
   (the bundled libraries are x86-only).
5. Build and run the `SpaceShoot` target — assets and DLLs are copied next to
   the executable, and the debugger working directory is preconfigured.

## Credits

Built by **Mihai Iftode**, **Danyang Wang**, and **Rares Popa**.

## License

Copyright © 2017 Mihai Iftode, Danyang Wang, Rares Popa.

Licensed under the **MIT License** — see [LICENSE.md](LICENSE.md).
