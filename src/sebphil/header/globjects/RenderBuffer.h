#pragma once

class RenderBuffer {

private:

	unsigned int glID;

	void generateRenderbuffer();

public:

	RenderBuffer();
	RenderBuffer(unsigned int format, unsigned int width, unsigned int height);
	~RenderBuffer();

	void setData(unsigned int format, unsigned int width, unsigned int height);

	void bind();
	void unbind();

	unsigned int getID() const;

};
