#pragma once

// Class wrapper to provide an optional scoped object on the stack without any dynamic allocations.
template<typename ScopedObject>
class OptionalScopedObject
{
public:
	// Creates a new inactive object. The underlying ScopedObject type is not initialized!
	OptionalScopedObject() : m_active(false)
	{
	}

	// If the underlying ScopedObject was created via construct, it will be deconstructed.
	~OptionalScopedObject()
	{
		destruct();
	}

	OptionalScopedObject(OptionalScopedObject&) = delete;
	void operator = (OptionalScopedObject&) = delete;

	// Constructs the object. Will destruct the object first if there was already one.
	template<typename... Args>
	void construct(Args... args)
	{
		if (m_active)
		{
			(*this)->~ScopedObject();
		}
		new (m_memory) ScopedObject(args...);
		m_active = true;
	}

	// Destructs the underlying object if one has been previously constructed.
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

	// Weather the underlying object is constructed or not.
	operator bool() const
	{
		return m_active;
	}

private:
	char m_memory[sizeof(ScopedObject)];
	bool m_active;
};