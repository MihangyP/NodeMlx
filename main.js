const node_mlx = require("./node_mlx.node");
const WIDTH = 800;
const HEIGHT = 600;

const mlx_ptr = node_mlx.mlxInit();
if (!mlx_ptr)
	new Error("Error on calling mlxInit");
const mlx_win = node_mlx.mlxNewWindow(mlx_ptr, WIDTH, HEIGHT, "Hello World");
if (!mlx_win)
	new Error("Error on calling mlxNewWindow");
for (let y = 0; y < HEIGHT; ++y) {
	for (let x = 0; x < WIDTH; ++x) {
		node_mlx.mlxPixelPut(mlx_ptr, mlx_win, x, y, 0x00FF0000);
	}
}

node_mlx.mlxLoop(mlx_ptr);
