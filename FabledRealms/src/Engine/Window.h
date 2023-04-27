#pragma once

class Window
{
public:
	Window(const char* title, int width, int height);
	~Window();

	inline void* GetWindowHandle() const { return m_Window; }

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline float GetAspectRatio() const { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }


	inline bool IsVSync() const { return m_VSync; }
	void SetVSync(bool enable);

	void OnUpdate();
	
	double GetCurrentTime();

	//Temporary
	bool WindowShouldClose();

private:

	void* m_Window;

	const char* m_Title;
	int m_Width;
	int m_Height;
	bool m_VSync;
};