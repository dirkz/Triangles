# Triangles

Vertex pipelines using SDL3 GPU.

## Dependencies

This project uses git submodules for most, but not all, dependencies.

```
git clone --recurse-submodules git@github.com:dirkz/Triangles.git
```

If you checked out without submodules and want to correct this, or just want
to bring versions to the official state (e.g. after having changed branches):

```
git submodule update --init --recursive
```

### Boost

At least boost header files must be present on your system and set up so cmake
(`find_package(Boost REQUIRED)`) finds them.
