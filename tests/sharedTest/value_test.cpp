#include "gtest/gtest.h"

#include "shared/value.h"

enum EnumA {
    EnumAOne,
    EnumATwo,
    EnumAThree,
};

enum EnumB : unsigned {
    EnumBOne,
    EnumBTwo,
    EnumBThree,
};

struct TestStruct {
    int a, b, c;
};

class TestClass {
public:
    TestClass() {}

protected:
    int a, b, c;
};

TEST(SharedValueTest, ctor)
{
    // constant
    EXPECT_NO_THROW(Value v);
    EXPECT_NO_THROW(Value v(true));

    EXPECT_NO_THROW(Value v(0.f));
    EXPECT_NO_THROW(Value v(-1));
    EXPECT_NO_THROW(Value v(NULL));
    EXPECT_NO_THROW(Value v(static_cast<const void*>(NULL)));
    EXPECT_NO_THROW(Value v("test"));
    EXPECT_NO_THROW(Value v(0u));

    EXPECT_NO_THROW(Value v(EnumAOne));
    EXPECT_NO_THROW(Value v(EnumBOne));

    // scalar and pointers
    int testInt = 12345;
    EXPECT_NO_THROW(Value v(testInt));

    const int cTestInt = testInt;
    EXPECT_NO_THROW(Value v(cTestInt));

    const void* pTestInt = &testInt;
    EXPECT_NO_THROW(Value v(pTestInt));

    const int* cpTestInt = &testInt;
    EXPECT_NO_THROW(Value v(cpTestInt));

    int& rTestInt = testInt;
    EXPECT_NO_THROW(Value v(rTestInt));

    const int& crTestInt = testInt;
    EXPECT_NO_THROW(Value v(crTestInt));

    void* ptr = &pTestInt;
    EXPECT_NO_THROW(Value v(ptr));

    // enum
    EnumA ea = EnumAThree;
    EnumB eb = EnumBTwo;
    EXPECT_NO_THROW(Value v(ea));
    EXPECT_NO_THROW(Value v(eb));

    // struct
    TestStruct ts;
    const TestStruct cts = { 1, 2, 3 };
    TestStruct* pts = &ts;
    const TestStruct* cpts = &ts;
    TestStruct& rts = ts;
    const TestStruct& crts = ts;

    EXPECT_NO_THROW(Value v(new TestStruct));
    EXPECT_NO_THROW(Value v(pts));
    EXPECT_NO_THROW(Value v(cpts));

    // class
    TestClass tc;
    const TestClass ctc;
    TestClass* ptc = &tc;
    const TestClass* cptc = &tc;
    TestClass& rtc = tc;
    const TestClass& crtc = tc;

    EXPECT_NO_THROW(Value v(new TestClass()));
    EXPECT_NO_THROW(Value v(ptc));
    EXPECT_NO_THROW(Value v(cptc));

    // nullptr not supported
    //EXPECT_ANY_THROW(Value v(nullptr));
    EXPECT_NO_THROW(Value v(Value::Null));

    // compiler errors (good)
    //EXPECT_ANY_THROW(Value v(0.));
    //EXPECT_ANY_THROW(Value v(0ll));
    //EXPECT_ANY_THROW(Value v(std::string("test")));
    //EXPECT_ANY_THROW(Value v(std::make_shared<int>()));

    //EXPECT_ANY_THROW(Value v(ts));
    //EXPECT_ANY_THROW(Value v(cts));
    //EXPECT_ANY_THROW(Value v(rts));
    //EXPECT_ANY_THROW(Value vv(crts));

    //EXPECT_ANY_THROW(Value v(tc));
    //EXPECT_ANY_THROW(Value v(ctc));
    //EXPECT_ANY_THROW(Value v(rtc));
    //EXPECT_ANY_THROW(Value v(crtc));
}

TEST(SharedValueTest, operatorAssgn)
{
    // bool
    Value vb = true;
    EXPECT_NO_THROW(vb = false);
    EXPECT_ANY_THROW(vb = 1.0f);
    EXPECT_ANY_THROW(vb = 17);
    EXPECT_ANY_THROW(vb = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vb = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vb = Value::Null);
    //EXPECT_ANY_THROW(vb = nullptr);
    EXPECT_ANY_THROW(vb = 0x113Fu);
    EXPECT_ANY_THROW(vb = EnumAOne);

    // float
    Value vf = .0f;
    EXPECT_ANY_THROW(vf = false);
    EXPECT_NO_THROW(vf = 1.0f);
    EXPECT_ANY_THROW(vf = 17);
    EXPECT_ANY_THROW(vf = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vf = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vf = Value::Null);
    //EXPECT_ANY_THROW(vf = nullptr);
    EXPECT_ANY_THROW(vf = 0x113Fu);
    EXPECT_ANY_THROW(vf = EnumAOne);

    // int
    Value vi = 0;
    EXPECT_ANY_THROW(vi = false);
    EXPECT_ANY_THROW(vi = 1.0f);
    EXPECT_NO_THROW(vi = 17);
    EXPECT_ANY_THROW(vi = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vi = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vi = Value::Null);
    //EXPECT_ANY_THROW(vi = nullptr);
    EXPECT_ANY_THROW(vi = 0x113Fu);
    EXPECT_ANY_THROW(vi = EnumAOne);

    // const void *
    int something = 10;
    Value vp = reinterpret_cast<const void*>(&something);
    EXPECT_ANY_THROW(vp = false);
    EXPECT_ANY_THROW(vp = 1.0f);
    EXPECT_ANY_THROW(vp = 17);
    EXPECT_NO_THROW(vp = static_cast<const void*>(NULL));
    EXPECT_NO_THROW(vp = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vp = Value::Null);
    EXPECT_ANY_THROW(vp = NULL);
    //EXPECT_ANY_THROW(vp = nullptr);
    EXPECT_ANY_THROW(vp = 0x113Fu);
    EXPECT_ANY_THROW(vp = EnumAOne);

    // const char *
    Value vs = "test";
    EXPECT_ANY_THROW(vs = false);
    EXPECT_ANY_THROW(vs = 1.0f);
    EXPECT_ANY_THROW(vs = 17);
    EXPECT_ANY_THROW(vs = static_cast<const void*>(NULL));
    EXPECT_NO_THROW(vs = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vs = Value::Null);
    EXPECT_ANY_THROW(vp = NULL);
    //EXPECT_ANY_THROW(vs = nullptr);
    EXPECT_ANY_THROW(vs = 0x113Fu);
    EXPECT_ANY_THROW(vs = EnumAOne);

    // unsigned
    Value vu = 10u;
    EXPECT_ANY_THROW(vu = false);
    EXPECT_ANY_THROW(vu = 1.0f);
    EXPECT_ANY_THROW(vu = 17);
    EXPECT_ANY_THROW(vu = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vu = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vu = Value::Null);
    //EXPECT_ANY_THROW(vu = nullptr);
    EXPECT_NO_THROW(vu = 0x113Fu);
    EXPECT_ANY_THROW(vu = EnumAOne);

    // enum
    Value ve = EnumBTwo;
    EXPECT_ANY_THROW(ve = false);
    EXPECT_ANY_THROW(ve = 1.0f);
    EXPECT_ANY_THROW(ve = 17);
    EXPECT_ANY_THROW(ve = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(ve = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(ve = Value::Null);
    //EXPECT_ANY_THROW(ve = nullptr);
    EXPECT_ANY_THROW(ve = 0x113Fu);
    EXPECT_ANY_THROW(ve = EnumAOne);
    EXPECT_NO_THROW(ve = EnumBThree);
}

TEST(SharedValueTest, as)
{
    Value vb = true;
    EXPECT_NO_THROW(auto x = vb.as<bool>());
    EXPECT_ANY_THROW(auto x = vb.as<float>());
    EXPECT_ANY_THROW(auto x = vb.as<int>());
    EXPECT_ANY_THROW(auto x = vb.as<const void*>());
    EXPECT_ANY_THROW(auto x = vb.as<const char*>());
    //EXPECT_ANY_THROW(auto x = vb.as<std::nullptr_t>());
    EXPECT_ANY_THROW(auto x = vb.as<unsigned>());
    EXPECT_ANY_THROW(auto x = vb.as<EnumA>());

    Value vf = 646.3342f;
    EXPECT_ANY_THROW(auto x = vf.as<bool>());
    EXPECT_NO_THROW(auto x = vf.as<float>());
    EXPECT_ANY_THROW(auto x = vf.as<int>());
    EXPECT_ANY_THROW(auto x = vf.as<const void*>());
    EXPECT_ANY_THROW(auto x = vf.as<const char*>());
    //EXPECT_ANY_THROW(auto x = vf.as<std::nullptr_t>());
    EXPECT_ANY_THROW(auto x = vf.as<unsigned>());
    EXPECT_ANY_THROW(auto x = vf.as<EnumA>());

    Value vi = -214;
    EXPECT_ANY_THROW(auto x = vi.as<bool>());
    EXPECT_ANY_THROW(auto x = vi.as<float>());
    EXPECT_NO_THROW(auto x = vi.as<int>());
    EXPECT_ANY_THROW(auto x = vi.as<const void*>());
    EXPECT_ANY_THROW(auto x = vi.as<const char*>());
    //EXPECT_ANY_THROW(auto x = vi.as<std::nullptr_t>());
    EXPECT_ANY_THROW(auto x = vi.as<unsigned>());
    EXPECT_ANY_THROW(auto x = vi.as<EnumA>());

    Value vp = static_cast<const void*>(NULL);
    EXPECT_ANY_THROW(auto x = vp.as<bool>());
    EXPECT_ANY_THROW(auto x = vp.as<float>());
    EXPECT_ANY_THROW(auto x = vp.as<int>());
    EXPECT_NO_THROW(auto x = vp.as<const void*>());
    //EXPECT_ANY_THROW(auto x = vp.as<void*>());
    EXPECT_ANY_THROW(auto x = vp.as<const char*>());
    //EXPECT_ANY_THROW(auto x = vp.as<char*>());
    //EXPECT_ANY_THROW(auto x = vp.as<std::nullptr_t>());
    EXPECT_ANY_THROW(auto x = vp.as<unsigned>());
    EXPECT_ANY_THROW(auto x = vp.as<EnumA>());

    Value vs = "testing";
    EXPECT_ANY_THROW(auto x = vs.as<bool>());
    EXPECT_ANY_THROW(auto x = vs.as<float>());
    EXPECT_ANY_THROW(auto x = vs.as<int>());
    EXPECT_ANY_THROW(auto x = vs.as<const void*>());
    //EXPECT_ANY_THROW(auto x = vs.as<void*>());
    EXPECT_NO_THROW(auto x = vs.as<const char*>());
    //EXPECT_ANY_THROW(auto x = vs.as<char*>());
    //EXPECT_NO_THROW(auto x = vs.as<std::nullptr_t>());
    EXPECT_ANY_THROW(auto x = vs.as<unsigned>());
    EXPECT_ANY_THROW(auto x = vs.as<EnumA>());

    Value vu = 325u;
    EXPECT_ANY_THROW(auto x = vu.as<bool>());
    EXPECT_ANY_THROW(auto x = vu.as<float>());
    EXPECT_ANY_THROW(auto x = vu.as<int>());
    EXPECT_ANY_THROW(auto x = vu.as<const void*>());
    EXPECT_ANY_THROW(auto x = vu.as<const char*>());
    //EXPECT_ANY_THROW(auto x = vu.as<std::nullptr_t>());
    EXPECT_NO_THROW(auto x = vu.as<unsigned>());
    EXPECT_ANY_THROW(auto x = vu.as<EnumA>());

    EXPECT_ANY_THROW(auto x = vu.as<TestStruct>());
    EXPECT_ANY_THROW(auto x = vu.as<TestClass>());

    Value ve = EnumAOne;
    EXPECT_ANY_THROW(auto x = ve.as<bool>());
    EXPECT_ANY_THROW(auto x = ve.as<float>());
    EXPECT_ANY_THROW(auto x = ve.as<int>());
    EXPECT_ANY_THROW(auto x = ve.as<const void*>());
    EXPECT_ANY_THROW(auto x = ve.as<const char*>());
    EXPECT_ANY_THROW(auto x = ve.as<std::nullptr_t>());
    EXPECT_ANY_THROW(auto x = ve.as<unsigned>());
    EXPECT_NO_THROW(auto x = ve.as<EnumA>());
    EXPECT_ANY_THROW(auto x = ve.as<EnumB>());
}

TEST(SharedValueTest, cast)
{
    // bool
    Value vb = false;
    EXPECT_NO_THROW(bool l = vb);
    EXPECT_ANY_THROW(float l = vb);
    EXPECT_ANY_THROW(int l = vb);
    EXPECT_ANY_THROW(const void* l = vb);
    EXPECT_ANY_THROW(const char* l = vb);
    EXPECT_ANY_THROW(void* l = vb);
    EXPECT_ANY_THROW(char* l = vb);
    EXPECT_ANY_THROW(std::nullptr_t l = vb);
    EXPECT_ANY_THROW(unsigned l = vb);
    EXPECT_ANY_THROW(EnumB l = vb);

    // float
    Value vf = 34.234f;
    EXPECT_ANY_THROW(bool l = vf);
    EXPECT_NO_THROW(float l = vf);
    EXPECT_ANY_THROW(int l = vf);
    EXPECT_ANY_THROW(const void* l = vf);
    EXPECT_ANY_THROW(const char* l = vf);
    EXPECT_ANY_THROW(void* l = vf);
    EXPECT_ANY_THROW(char* l = vf);
    EXPECT_ANY_THROW(std::nullptr_t l = vf);
    EXPECT_ANY_THROW(unsigned l = vf);
    EXPECT_ANY_THROW(EnumB l = vf);

    // int
    Value vi = 56;
    EXPECT_ANY_THROW(bool l = vi);
    EXPECT_ANY_THROW(float l = vi);
    EXPECT_NO_THROW(int l = vi);
    EXPECT_ANY_THROW(const void* l = vi);
    EXPECT_ANY_THROW(const char* l = vi);
    EXPECT_ANY_THROW(void* l = vi);
    EXPECT_ANY_THROW(char* l = vi);
    EXPECT_ANY_THROW(std::nullptr_t l = vi);
    EXPECT_ANY_THROW(unsigned l = vi);
    EXPECT_ANY_THROW(EnumB l = vi);

    // const void *
    int n = 12;
    const void* pn = &pn;
    Value vp = pn;
    EXPECT_ANY_THROW(bool l = vp);
    EXPECT_ANY_THROW(float l = vp);
    EXPECT_ANY_THROW(int l = vp);
    EXPECT_NO_THROW(const void* l = vp);
    EXPECT_ANY_THROW(const char* l = vp);
    EXPECT_ANY_THROW(void* l = vp);
    EXPECT_ANY_THROW(char* l = vp);
    EXPECT_ANY_THROW(std::nullptr_t l = vp);
    EXPECT_ANY_THROW(unsigned l = vp);
    EXPECT_ANY_THROW(EnumB l = vp);

    // const char *
    Value vs = "this is a test";
    EXPECT_ANY_THROW(bool l = vs);
    EXPECT_ANY_THROW(float l = vs);
    EXPECT_ANY_THROW(int l = vs);
    EXPECT_ANY_THROW(const void* l = vs);
    EXPECT_NO_THROW(const char* l = vs);
    EXPECT_ANY_THROW(void* l = vs);
    EXPECT_ANY_THROW(char* l = vs);
    EXPECT_ANY_THROW(std::nullptr_t l = vs);
    EXPECT_ANY_THROW(unsigned l = vs);
    EXPECT_ANY_THROW(EnumB l = vs);

    // unsigned
    Value vu = 243253u;
    EXPECT_ANY_THROW(bool l = vu);
    EXPECT_ANY_THROW(float l = vu);
    EXPECT_ANY_THROW(int l = vu);
    EXPECT_ANY_THROW(const void* l = vu);
    EXPECT_ANY_THROW(const char* l = vu);
    EXPECT_ANY_THROW(void* l = vu);
    EXPECT_ANY_THROW(char* l = vu);
    EXPECT_ANY_THROW(std::nullptr_t l = vu);
    EXPECT_NO_THROW(unsigned l = vu);
    EXPECT_ANY_THROW(EnumB l = vu);

    // enum
    Value ve = EnumAThree;
    EXPECT_ANY_THROW(bool l = ve);
    EXPECT_ANY_THROW(float l = ve);
    EXPECT_ANY_THROW(int l = ve);
    EXPECT_ANY_THROW(const void* l = ve);
    EXPECT_ANY_THROW(const char* l = ve);
    EXPECT_ANY_THROW(void* l = ve);
    EXPECT_ANY_THROW(char* l = ve);
    EXPECT_ANY_THROW(std::nullptr_t l = ve);
    EXPECT_ANY_THROW(unsigned l = ve);
    EXPECT_ANY_THROW(EnumB l = ve);
    EXPECT_NO_THROW(EnumA l = ve);
}
