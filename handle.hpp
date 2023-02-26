template<typename T>
class SmartHandle {
public:
    explicit SmartHandle(T h) noexcept = delete;
    T get() const noexcept = delete;
    operator bool() noexcept;
    ~SmartHandle() = delete;

private:

    T m_handle;
};


template<>
class SmartHandle<HANDLE> {
public:
    explicit SmartHandle(HANDLE h) noexcept
        : m_handle(h)
    {

    }

    HANDLE get() const noexcept
    {
        return m_handle;
    }

    operator bool() noexcept
    {
        return m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr;
    }

    ~SmartHandle()
    {
        if (m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr)
            CloseHandle(m_handle);
    }

private:

    HANDLE m_handle;
};

template<>
class SmartHandle<HBITMAP> {
public:
    explicit SmartHandle(HBITMAP h) noexcept
        : m_handle(h)
    {

    }

    HBITMAP get() const noexcept
    {
        return m_handle;
    }

    operator bool() noexcept
    {
        return m_handle != nullptr;
    }

    ~SmartHandle()
    {
        if (m_handle != nullptr)
            DeleteObject(m_handle);
    }

private:

    HBITMAP m_handle;
};