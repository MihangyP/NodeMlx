# NodeMlx

NodeMlx is a Node.js native addon written in C++ that provides bindings for the [minilibx](https://github.com/42paris/minilibx-linux) graphics library, allowing JavaScript programs to create windows and draw using the X11 system

## Build

```console
$ make
```

This generates a `node_mlx.node` shared object.

## Quick Start

```js
const mlx = require("./node_mlx.node");

const mlx_ptr = mlx.mlxInit();
const mlx_win = mlx.mlxNewWindow(mlx_ptr, 800, 600, "NodeMlx");

mlx.mlxPixelPut(mlx_ptr, mlx_win, 100, 100, 0xFFFFFF);

mlx.mlxLoop(mlx_ptr);
```
