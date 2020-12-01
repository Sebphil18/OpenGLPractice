#pragma once
class IndexBuffer {

private:

	unsigned int glID;
	unsigned int count;

	void generateBuffer();
	void deleteBuffer();

public:

	IndexBuffer();	
	IndexBuffer(unsigned int count, const void* data, unsigned int usage);
	~IndexBuffer();

	void bind();
	void unbind();

	void setData(unsigned int count, const void* data, unsigned int usage);

	unsigned int getCount() const;

};

