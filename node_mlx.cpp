#include <iostream>
#include <map>
#include <node.h>
extern "C" {
	#include <mlx.h>
}

void	MlxInit(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate	*isolate = args.GetIsolate();
	void *mlx_ptr = mlx_init();
	args.GetReturnValue().Set(
		v8::External::New(isolate, mlx_ptr)
	);
}

void	MlxNewWindow(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate	*isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	void	*mlx_ptr = v8::External::Cast(*args[0])->Value();
	int		width = args[1]->Int32Value(context).FromJust();
	int		height = args[2]->Int32Value(context).FromJust();
	v8::String::Utf8Value utf8(isolate, args[3]);
	std::string title = *utf8;
	void	*mlx_win = mlx_new_window(mlx_ptr, width, height, (char *)title.c_str());
	args.GetReturnValue().Set(
		v8::External::New(isolate, mlx_win)
	);
}	

void	MlxClearWindow(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate	*isolate = args.GetIsolate();
	void	*mlx_ptr = v8::External::Cast(*args[0])->Value();
	void	*mlx_win = v8::External::Cast(*args[1])->Value();
	int		status = mlx_clear_window(mlx_ptr, mlx_win);
	args.GetReturnValue().Set(
		v8::Integer::New(isolate, status)
	);
}

void MlxPixelPut(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();

	if (args.Length() < 5 ||
		!args[0]->IsExternal() ||
		!args[1]->IsExternal() ||
		!args[2]->IsNumber() ||
		!args[3]->IsNumber() ||
		!args[4]->IsNumber())
	{
		isolate->ThrowException(
			v8::String::NewFromUtf8(isolate, "Invalid arguments").ToLocalChecked()
		);
		return;
	}

	void* mlx_ptr = v8::Local<v8::External>::Cast(args[0])->Value();
	void* mlx_win = v8::Local<v8::External>::Cast(args[1])->Value();
	int x = args[2]->Int32Value(context).FromJust();
	int y = args[3]->Int32Value(context).FromJust();
	uint32_t color = args[4]->Uint32Value(context).FromJust();

	int status = mlx_pixel_put(mlx_ptr, mlx_win, x, y, color);

	args.GetReturnValue().Set(
		v8::Integer::New(isolate, status)
	);
}

void	MlxLoop(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate	*isolate = args.GetIsolate();
	void	*mlx_ptr = v8::External::Cast(*args[0])->Value();
	int 	status = mlx_loop(mlx_ptr);
	args.GetReturnValue().Set(
		v8::Integer::New(isolate, status)
	);
}

std::map<void*, v8::Persistent<v8::Function>> key_callbacks;

int KeyHookCallback() {
    for (auto &pair : key_callbacks) {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Function> cb = v8::Local<v8::Function>::New(isolate, pair.second);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        cb->Call(context, v8::Undefined(isolate), 0, nullptr).ToLocalChecked();
    }
    return 0;
}

void MlxKeyHook(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) return;
    if (!args[0]->IsExternal() || !args[1]->IsFunction()) return;
    void* win_ptr = v8::Local<v8::External>::Cast(args[0])->Value();
    v8::Local<v8::Function> js_cb = v8::Local<v8::Function>::Cast(args[1]);
    key_callbacks[win_ptr].Reset(isolate, js_cb);
    mlx_key_hook(win_ptr, KeyHookCallback, nullptr);
    args.GetReturnValue().Set(v8::Undefined(isolate));
}

void Init(v8::Local<v8::Object> exports) {
	NODE_SET_METHOD(exports, "mlxInit", MlxInit);
	NODE_SET_METHOD(exports, "mlxNewWindow", MlxNewWindow);
	NODE_SET_METHOD(exports, "mlxClearWindow", MlxClearWindow);
	NODE_SET_METHOD(exports, "mlxPixelPut", MlxPixelPut);
	NODE_SET_METHOD(exports, "mlxLoop", MlxLoop);
	NODE_SET_METHOD(exports, "mlxKeyHook", MlxKeyHook);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init) // N.B.: no semi-colon, this is not a function
