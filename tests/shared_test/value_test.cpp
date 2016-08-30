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

TEST(SharedValueTest, ctorBool)
{
    bool b = true;
    EXPECT_NO_THROW(Value v(b));
    EXPECT_NO_THROW(Value v(false));

    Value v(false);
    EXPECT_EQ(v.type(), typeid(bool));
    EXPECT_EQ(v.as<bool>(), false);

    Value v2(true);
    EXPECT_EQ(v2.type(), typeid(bool));
    EXPECT_EQ(v2.as<bool>(), true);

    Value v3(b);
    EXPECT_EQ(v3.type(), typeid(bool));
    EXPECT_EQ(v3.as<bool>(), true);

    b = false;
    Value v4(b);
    EXPECT_EQ(v4.type(), typeid(bool));
    EXPECT_EQ(v4.as<bool>(), false);

    float f = 11.44;
    Value v5(!(f - 111));
    EXPECT_EQ(v5.type(), typeid(bool));
    EXPECT_EQ(v5.as<bool>(), false);

    Value v6(!!0.1f);
    EXPECT_EQ(v6.type(), typeid(bool));
    EXPECT_EQ(v6.as<bool>(), true);

    EXPECT_NO_THROW(Value v7(v6));
    Value v7(v6);
    EXPECT_EQ(v7.type(), typeid(bool));
    EXPECT_EQ(v7.as<bool>(), true);
}

TEST(SharedValueTest, ctorFloat)
{
    float f = 11.11;
    double d = 22.22;
    EXPECT_NO_THROW(Value v(1.0f));
    EXPECT_NO_THROW(Value v(1.0));
    EXPECT_NO_THROW(Value v(f));
    EXPECT_NO_THROW(Value v(d));

    Value v(1 / 33.0f);
    EXPECT_EQ(v.as<float>(), 1 / 33.0f);
    Value v1(1 / 33.0);
    EXPECT_EQ(v.as<float>(), 1 / 33.0f);
    EXPECT_EQ(v.as<double>(), 1 / 33.0f);
    EXPECT_NE(v.as<double>(), 1 / 33.0);

    Value v2(f);
    EXPECT_EQ(v2.as<float>(), 11.11f);

    Value v3(d);
    EXPECT_EQ(v3.as<float>(), 22.22f);
    EXPECT_EQ(v3.as<double>(), 22.22f);
    EXPECT_NE(v3.as<double>(), 22.22);

    EXPECT_NO_THROW(Value v4(v3));
    Value v4(v3);
    EXPECT_EQ(v4.as<float>(), 22.22f);
    EXPECT_EQ(v4.as<double>(), 22.22f);

    EXPECT_EQ(v.type(), typeid(float));
    EXPECT_EQ(v2.type(), typeid(float));
    EXPECT_EQ(v3.type(), typeid(float));
    EXPECT_EQ(v4.type(), typeid(float));
}

TEST(SharedValueTest, ctorInt)
{
    int i = -324523;
    unsigned u = 0x334234u;
    char c = -22;
    short s = 289;
    unsigned char uc = 244;
    unsigned short us = 3467;
    long long ll = -20398574395872907ll;
    unsigned long long ull = 985734209239847656ull;

    EXPECT_NO_THROW(Value v(i));
    EXPECT_NO_THROW(Value v(u));
    EXPECT_NO_THROW(Value v(c));
    EXPECT_NO_THROW(Value v(s));
    EXPECT_NO_THROW(Value v(uc));
    EXPECT_NO_THROW(Value v(us));
    EXPECT_NO_THROW(Value v(ll));
    EXPECT_NO_THROW(Value v(ull));

    Value vi(i);
    EXPECT_EQ(vi.as<int>(), i);
    Value vu(u);
    EXPECT_EQ(vu.as<unsigned>(), u);
    Value vc(c);
    EXPECT_EQ(vc.as<char>(), c);
    Value vs(s);
    EXPECT_EQ(vs.as<short>(), s);
    Value vuc(uc);
    EXPECT_EQ(vuc.as<unsigned char>(), uc);
    Value vus(us);
    EXPECT_EQ(vus.as<unsigned short>(), us);
    Value vll(ll);
    EXPECT_NE(vll.as<long long>(), ll); // 32 bit storage overfflow
    EXPECT_EQ(vll.as<long long>(), static_cast<long long>(int(ll)));
    Value vull(ull);
    EXPECT_NE(vull.as<unsigned long long>(), ull); // 32 bit storage overfflow
    EXPECT_EQ(vull.as<unsigned long long>(), static_cast<unsigned long long>(int(ull)));

    Value vllNoOverflow(-42424ll); // no 32 bit storage overflow
    EXPECT_EQ(vllNoOverflow.as<long long>(), -42424ll);
    Value vullNoOverflow(0x43424242ull); // no 32 bit storage overflow
    EXPECT_EQ(vullNoOverflow.as<unsigned long long>(), 0x43424242ull);

    EXPECT_EQ(vi.type(), typeid(int));
    EXPECT_EQ(vu.type(), typeid(int));
    EXPECT_EQ(vc.type(), typeid(int));
    EXPECT_EQ(vs.type(), typeid(int));
    EXPECT_EQ(vuc.type(), typeid(int));
    EXPECT_EQ(vus.type(), typeid(int));
    EXPECT_EQ(vll.type(), typeid(int));
    EXPECT_EQ(vull.type(), typeid(int));

    // copy
    EXPECT_NO_THROW(Value v(vi));
    EXPECT_NO_THROW(Value v(vu));
    EXPECT_NO_THROW(Value v(vc));
    EXPECT_NO_THROW(Value v(vs));
    EXPECT_NO_THROW(Value v(vuc));
    EXPECT_NO_THROW(Value v(vus));
    EXPECT_NO_THROW(Value v(vll));
    EXPECT_NO_THROW(Value v(vull));

    Value vi2(vi);
    EXPECT_EQ(vi2.as<int>(), i);
    Value vu2(vu);
    EXPECT_EQ(vu2.as<unsigned>(), u);
    Value vc2(vc);
    EXPECT_EQ(vc2.as<char>(), c);
    Value vs2(vs);
    EXPECT_EQ(vs2.as<short>(), s);
    Value vuc2(vuc);
    EXPECT_EQ(vuc2.as<unsigned char>(), uc);
    Value vus2(vus);
    EXPECT_EQ(vus2.as<unsigned short>(), us);
    Value vll2(vll);
    EXPECT_NE(vll.as<long long>(), ll);
    EXPECT_EQ(vll2.as<long long>(), static_cast<long long>(int(vll)));
    Value vull2(vull);
    EXPECT_NE(vull.as<unsigned long long>(), ull);
    EXPECT_EQ(vull2.as<unsigned long long>(), static_cast<unsigned long long>(int(vull)));

    EXPECT_EQ(vi2.type(), typeid(int));
    EXPECT_EQ(vu2.type(), typeid(int));
    EXPECT_EQ(vc2.type(), typeid(int));
    EXPECT_EQ(vs2.type(), typeid(int));
    EXPECT_EQ(vuc2.type(), typeid(int));
    EXPECT_EQ(vus2.type(), typeid(int));
    EXPECT_EQ(vll2.type(), typeid(int));
    EXPECT_EQ(vull2.type(), typeid(int));
}

TEST(SharedValueTest, ctorPtr)
{
    void* p;
    const void* cp;
    int* pi;
    const int* cpi;
    bool* pb;
    const bool* cpb;
    TestStruct* pts;
    const TestStruct* cpts;
    void* pn = NULL;
    void* pnullptr = nullptr;

    EXPECT_NO_THROW(Value v(p));
    EXPECT_NO_THROW(Value v(cp));
    EXPECT_NO_THROW(Value v(pi));
    EXPECT_NO_THROW(Value v(cpi));
    EXPECT_NO_THROW(Value v(pb));
    EXPECT_NO_THROW(Value v(cpb));
    EXPECT_NO_THROW(Value v(pts));
    EXPECT_NO_THROW(Value v(cpts));
    EXPECT_NO_THROW(Value v(pn));
    EXPECT_NO_THROW(Value v(pnullptr));
    EXPECT_NO_THROW(Value v(nullptr));
    EXPECT_NO_THROW(Value v(NULL)); // probably will be int(0) or _null(0) (bad)

    Value vp(p);
    Value vcp(cp);
    Value vpi(pi);
    Value vcpi(cpi);
    Value vpb(pb);
    Value vcpb(cpb);
    Value vpts(pts);
    Value vcpts(cpts);
    Value vpn(pn);
    Value vnullptr(nullptr);
    Value vNULL(NULL);

    EXPECT_EQ(vp.type(), typeid(const void*));
    EXPECT_EQ(vcp.type(), typeid(const void*));
    EXPECT_EQ(vpi.type(), typeid(const int*));
    EXPECT_EQ(vcpi.type(), typeid(const int*));
    EXPECT_EQ(vpb.type(), typeid(const bool*));
    EXPECT_EQ(vcpb.type(), typeid(const bool*));
    EXPECT_EQ(vpts.type(), typeid(const TestStruct*));
    EXPECT_EQ(vcpts.type(), typeid(const TestStruct*));
    EXPECT_EQ(vpn.type(), typeid(const void*));
    EXPECT_EQ(vnullptr.type(), typeid(const void*));

    EXPECT_NE(vNULL.type(), typeid(const void*));
    EXPECT_EQ(vNULL.type(), typeid(NULL));
}

TEST(SharedValueTest, ctorNulls)
{
    EXPECT_NO_THROW(Value v(0));
    EXPECT_NO_THROW(Value v(0.0));
    EXPECT_NO_THROW(Value v(0.0f));
    EXPECT_NO_THROW(Value v('\0'));
    EXPECT_NO_THROW(Value v(false));
    EXPECT_NO_THROW(Value v(NULL));
    EXPECT_NO_THROW(Value v(nullptr));
    EXPECT_NO_THROW(Value v(Value::Null));

    Value v0(0);
    EXPECT_EQ(v0.type(), typeid(int));
    Value v0dot0(0.0);
    EXPECT_EQ(v0dot0.type(), typeid(float));
    Value v0s('\0');
    EXPECT_EQ(v0s.type(), typeid(int));
    Value vfalse(false);
    EXPECT_EQ(vfalse.type(), typeid(bool));
    Value vNULL(NULL);
    EXPECT_EQ(vNULL.type(), typeid(NULL));
    Value vValueNull(Value::Null);
    EXPECT_EQ(vValueNull.type(), typeid(Value::Null));

    EXPECT_NO_THROW(vValueNull = "Hello"); // not locked
    EXPECT_EQ(vValueNull.type(), typeid(const char*)); // now locked
    EXPECT_ANY_THROW(vValueNull = 1);
}

TEST(SharedValueTest, ctorMisc)
{
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

    EXPECT_NO_THROW(Value v(0.));
    EXPECT_NO_THROW(Value v(0ll));

    // compiler errors (good)
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
    EXPECT_EQ(bool(vb), true);
    EXPECT_NO_THROW(vb = false);
    EXPECT_EQ(bool(vb), false);

    EXPECT_ANY_THROW(vb = 1.0f);
    EXPECT_ANY_THROW(vb = 17);
    EXPECT_ANY_THROW(vb = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vb = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vb = Value::Null);
    EXPECT_EQ(bool(vb), false);
    //EXPECT_ANY_THROW(vb = nullptr);
    EXPECT_ANY_THROW(vb = 0x113Fu);
    EXPECT_ANY_THROW(vb = EnumAOne);

    Value vb2(true);
    EXPECT_EQ(bool(vb2), true);
    vb = false;
    EXPECT_NO_THROW(vb = vb2);
    EXPECT_EQ(bool(vb), bool(vb2));
    EXPECT_EQ(bool(vb), true);

    // float
    Value vf = .0f;
    EXPECT_ANY_THROW(vf = false);
    EXPECT_NO_THROW(vf = 1.0f);
    EXPECT_ANY_THROW(vf = 17);
    EXPECT_ANY_THROW(vf = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vf = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vf = Value::Null);
    EXPECT_ANY_THROW(vf = nullptr);
    EXPECT_ANY_THROW(vf = 0x113Fu);
    EXPECT_ANY_THROW(vf = EnumAOne);

    EXPECT_NO_THROW(vf = 17 / 11.f);
    EXPECT_EQ(float(vf), 17 / 11.f);

    Value vf2(15 / 114.f);
    EXPECT_EQ(float(vf2), 15 / 114.f);
    EXPECT_ANY_THROW(vf = vb);
    EXPECT_NO_THROW(vf = vf2);
    EXPECT_EQ(float(vf), float(vf2));
    EXPECT_EQ(float(vf), 15 / 114.f);

    Value vf3;
    EXPECT_EQ(vf3.type(), typeid(Value::NullType));
    EXPECT_NO_THROW(vf3 = 33 / 55.f);
    EXPECT_NE(vf3.type(), typeid(Value::NullType));
    EXPECT_EQ(vf3.type(), typeid(float));
    EXPECT_NO_THROW(vf3.as<float>());
    EXPECT_EQ(float(vf3), 33 / 55.f);
    Value vf4;
    EXPECT_NO_THROW(vf4 = vf3);
    EXPECT_EQ(float(vf4), 33 / 55.f);

    // int
    Value vi = 0;
    EXPECT_ANY_THROW(vi = false);
    EXPECT_ANY_THROW(vi = 1.0f);
    EXPECT_NO_THROW(vi = 17);
    EXPECT_ANY_THROW(vi = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vi = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vi = Value::Null);
    //EXPECT_ANY_THROW(vi = nullptr);
    EXPECT_NO_THROW(vi = 0x113Fu);
    EXPECT_ANY_THROW(vi = EnumAOne);

    // const void *
    int something = 10;
    Value vp = reinterpret_cast<const void*>(&something);
    EXPECT_ANY_THROW(vp = false);
    EXPECT_ANY_THROW(vp = 1.0f);
    EXPECT_ANY_THROW(vp = 17);
    EXPECT_NO_THROW(vp = static_cast<const void*>(NULL));
    EXPECT_ANY_THROW(vp = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vp = Value::Null);
    EXPECT_ANY_THROW(vp = NULL);
    EXPECT_NO_THROW(vp = nullptr);
    EXPECT_ANY_THROW(vp = 0x113Fu);
    EXPECT_ANY_THROW(vp = EnumAOne);

    EXPECT_ANY_THROW(vp = &something);
    EXPECT_NO_THROW(vp = static_cast<void *>(&something));
    Value vp2(vp);
    Value vp3;
    Value vp4;
    EXPECT_NO_THROW(vp3 = vp2);
    EXPECT_NO_THROW(vp4 = vp3);
    EXPECT_EQ(static_cast<const void*>(vp4), &something);

    Value vp5;
    Value vp6(nullptr);

    const char* str1 = "test";
    const void* cp = &str1;
    EXPECT_ANY_THROW(cp = vp5);

    vp5 = str1;
    EXPECT_NO_THROW(str1 = vp5);
    EXPECT_NO_THROW(vp5 = str1);
    EXPECT_ANY_THROW(vp5 = vp6);
    EXPECT_ANY_THROW(vp5 = cp);

    EXPECT_NO_THROW(vp6 = cp);
    EXPECT_ANY_THROW(vp6 = str1);
    EXPECT_NO_THROW(vp6 = static_cast<const void *>(str1));
    EXPECT_ANY_THROW(vp6 = vp5); // can't detect non-void ptr to void ptr value assignment

    Value vp7 = cp;
    EXPECT_ANY_THROW(vp7 = str1);
    EXPECT_NO_THROW(vp7 = nullptr);
    EXPECT_ANY_THROW(vp7 = vp5);

    // const char *
    Value vs = "test";
    EXPECT_ANY_THROW(vs = false);
    EXPECT_ANY_THROW(vs = 1.0f);
    EXPECT_ANY_THROW(vs = 17);
    EXPECT_ANY_THROW(vs = static_cast<const void*>(NULL));
    EXPECT_NO_THROW(vs = static_cast<const char*>(NULL));
    EXPECT_NO_THROW(vs = Value::Null);
    EXPECT_ANY_THROW(vs = NULL);
    EXPECT_ANY_THROW(vs = nullptr);
    EXPECT_ANY_THROW(vs = 0x113Fu);
    EXPECT_ANY_THROW(vs = EnumAOne);

    // unsigned
    Value vu = 10u;
    EXPECT_ANY_THROW(vu = false);
    EXPECT_ANY_THROW(vu = 1.0f);
    EXPECT_NO_THROW(vu = 17);
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
    EXPECT_NO_THROW(auto x = vf.as<double>());
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
    EXPECT_NO_THROW(auto x = vi.as<unsigned>());
    EXPECT_NO_THROW(auto x = vi.as<char>());
    EXPECT_NO_THROW(auto x = vi.as<unsigned char>());
    EXPECT_NO_THROW(auto x = vi.as<short>());
    EXPECT_NO_THROW(auto x = vi.as<unsigned short>());
    EXPECT_ANY_THROW(auto x = vi.as<const void*>());
    EXPECT_ANY_THROW(auto x = vi.as<const char*>());
    //EXPECT_ANY_THROW(auto x = vi.as<std::nullptr_t>());
    EXPECT_NO_THROW(auto x = vi.as<unsigned>());
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

    Value ve = EnumAOne;
    EXPECT_ANY_THROW(auto x = ve.as<bool>());
    EXPECT_ANY_THROW(auto x = ve.as<float>());
    EXPECT_ANY_THROW(auto x = ve.as<int>());
    EXPECT_ANY_THROW(auto x = ve.as<unsigned>());
    EXPECT_ANY_THROW(auto x = ve.as<const void*>());
    EXPECT_ANY_THROW(auto x = ve.as<const char*>());
    EXPECT_ANY_THROW(auto x = ve.as<std::nullptr_t>());
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
    EXPECT_NO_THROW(double l = vf);
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
    EXPECT_NO_THROW(unsigned l = vi);
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
    EXPECT_NO_THROW(int l = vu);
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

TEST(SharedValueTest, value)
{
    // bool
    Value vb = false;
    EXPECT_EQ(vb.as<bool>(), false);
    EXPECT_EQ(bool(vb), false);
    EXPECT_EQ(static_cast<bool>(vb), false);
    EXPECT_NO_THROW(bool b = vb);

    Value vb1(false);
    Value vb2(true);
    EXPECT_EQ(bool(vb1), false);
    EXPECT_EQ(bool(vb2), true);

    bool b = vb;
    EXPECT_EQ(b, false);

    EXPECT_NO_THROW(vb = true);

    EXPECT_EQ(vb.as<bool>(), true);
    EXPECT_EQ(bool(vb), true);
    EXPECT_EQ(static_cast<bool>(vb), true);

    b = vb;
    EXPECT_EQ(b, true);

    // float
    Value vf = 1 / 3.;

    EXPECT_EQ(vf.as<float>(), 1 / 3.f);
    EXPECT_EQ(float(vf), 1 / 3.f);
    EXPECT_EQ(static_cast<float>(vf), 1 / 3.f);

    EXPECT_EQ(vf.as<double>(), 1 / 3.f);
    EXPECT_EQ(double(vf), 1 / 3.f);
    EXPECT_EQ(static_cast<double>(vf), 1 / 3.f);

    Value vf2(1.0 / 7);
    Value vf3(1 / 8.f);
    Value vf4(vf3);
    EXPECT_EQ(float(vf2), 1.f / 7);
    EXPECT_EQ(float(vf3), 1.f / 8);
    EXPECT_EQ(float(vf4), 1.f / 8);

    EXPECT_NO_THROW(vf = 1 / 7.f);
    EXPECT_NO_THROW(float f = vf);
    EXPECT_NO_THROW(double d = vf);
    float f = vf;
    double d = vf;

    EXPECT_EQ(f, 1 / 7.f);
    EXPECT_EQ(d, 1 / 7.f);

    EXPECT_NO_THROW(vf = 14.245f);
    EXPECT_EQ(float(vf), 14.245f);
    EXPECT_EQ(double(vf), 14.245f);

    EXPECT_EQ(double(vf), double(14.245f));
    EXPECT_NE(double(vf), double(14.245));
    EXPECT_TRUE(float(vf) == float(14.245));

    // int
    Value vi = -56;

    EXPECT_EQ(vi.as<int>(), -56);
    EXPECT_EQ(int(vi), -56);
    EXPECT_EQ(static_cast<int>(vi), -56);
    EXPECT_NO_THROW(int i = vi);

    Value vi2(32);
    Value vi3(3530454);
    Value vi4(0xFF1110u);
    EXPECT_EQ(int(vi2), 32);
    EXPECT_EQ(int(vi3), 3530454);
    EXPECT_EQ(unsigned(vi4), 0xFF1110u);

    int i = vi;
    EXPECT_EQ(i, -56);

    EXPECT_NO_THROW(vi = 17);
    EXPECT_EQ(int(vi), 17);
    EXPECT_EQ(char(vi), 17);
    EXPECT_EQ(short(vi), 17);
    EXPECT_EQ(unsigned(vi), 17u);

    // pointer
    int test = 1;
    int test2 = 2;
    EXPECT_NO_THROW(Value vp(&test));
    Value vp(static_cast<void*>(&test));
    EXPECT_NO_THROW(const void* p = vp);
    const void* p = vp;
    EXPECT_EQ(static_cast<const void*>(vp), &test);
    EXPECT_ANY_THROW(vp = &test2);
    EXPECT_NO_THROW(vp = static_cast<const void *>(&test2));
    EXPECT_EQ(static_cast<const void*>(vp), &test2);
}
