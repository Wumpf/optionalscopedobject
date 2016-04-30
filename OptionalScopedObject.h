#pragma once

template<typename ScopedObject>
class OptionalScopedObject
{
public:
	OptionalScopedObject() : m_active(false)
	{
	}

	~OptionalScopedObject()
	{
		destruct();
	}

	OptionalScopedObject(OptionalScopedObject&) = delete;
	void operator = (OptionalScopedObject&) = delete;

	template<typename... Args>
	void construct(Args... args)
	{
		destruct();
		new (m_memory) ScopedObject(args...);
		m_active = true;
	}

	void destruct()
	{
		if (m_active)
		{
			(*this)->~ScopedObject();
			m_active = false;
		}
	}


	// Access to underlying object.
	const ScopedObject* operator -> () const
	{
		return reinterpret_cast<const ScopedObject*>(m_memory);
	}
	const ScopedObject& operator * () const
	{
		return *reinterpret_cast<const ScopedObject*>(m_memory);
	}
	ScopedObject* operator -> ()
	{
		return reinterpret_cast<ScopedObject*>(m_memory);
	}
	ScopedObject& operator * ()
	{
		return *reinterpret_cast<ScopedObject*>(m_memory);
	}

	// Weather the underlyting object is constructed or not.
	operator bool() const
	{
		return m_active;
	}

private:
	char m_memory[sizeof(ScopedObject)];
	bool m_active;
};