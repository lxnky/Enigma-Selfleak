namespace SonyDriverHelper {
	class api {
	public:
		static void Init();

		static void MouseMove(float x, float y);

		static bool GetKey(int id);
	};
}

//namespace mouse
//{
//	void run_mouse_loop();
//	bool initialize_mouse();
//	void move_mouse(int x, int y);
//	void mouse_click();
//
//}