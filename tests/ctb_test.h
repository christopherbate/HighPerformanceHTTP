/**
 * ctb_test.h
 * Author: Christopher Bate
 * 
 * This is my own custom testing framework, modeled (conceptually) after Google's C++ testing library.
 * This is a header-only framework, no need for libraries!
 * The basic idea is simply add several tests, and let the singleton take care of keeping track
 * of running, assertions, etc.
 */
#ifndef CTB_TEST
#define CTB_TEST
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

/**
 * Represents an individual test.
 */
class Test
{
  public:
    Test(std::string n, int (*f)())
    {
        name = n;
        func = f;
        fail = false;
    }
    void Run()
    {
        std::cout << name << ": " <<std::endl;
        if (func())
        {
            std::cout << "PASS" << std::endl;
            fail = false;
        }
        else
        {
            std::cout << "FAIL" << std::endl;
            fail = true;
        }
    }
    bool fail;
    int (*func)();
    std::string name;
};

/**
 * Represents a set of tests.
 */
class TestCase
{
  public:
    TestCase(std::string name) : m_name(name)
    {
        failCount = 0;
    }
    ~TestCase()
    {
        for (unsigned int i = 0; i < tests.size(); i++)
        {
            delete tests[i];
        }
    }
    void Run()
    {
        for (auto it = tests.begin(); it != tests.end(); it++)
        {
            (*it)->Run();
            if((*it)->fail){
                failCount++;
            }
        }
    }
    std::vector<Test *> tests;
    int failCount;
    std::string m_name;
};

/**
 * TestRunner
 * Runs all tests 
 */
class TestRunner
{
  public:
    /** Returns singleton instance */
    static TestRunner *GetRunner()
    {
        static TestRunner runner;
        return &runner;
    }
    ~TestRunner()
    {
        auto it = m_cases.begin();
        while (it != m_cases.end())
        {
            delete it->second;
            it++;
        }
    }

    void Run()
    {
        auto it = m_cases.begin();
        while (it != m_cases.end())
        {
            std::cout << it->first << std::endl;
            it->second->Run();
            it++;
        }
    }

    int GetRetCode(){
        return m_retCode;
    }

    void PrintSummary()
    {
        std::cout<<"======SUMMARY======"<<std::endl;
        for(auto it = m_cases.begin(); it != m_cases.end(); it++)
        {
            std::cout<< it->first <<":"<<
                it->second->tests.size()-it->second->failCount<<"/"<<
                it->second->tests.size()<< std::endl;
            if(it->second->failCount > 0){
                m_retCode = -1;
            }
        }
    }

    void AddTest(std::string caseName, std::string testName, int (*f)())
    {        
        auto testCase = m_cases.find(caseName);        
        if (testCase == m_cases.end())
        {     
            m_cases[caseName] = new TestCase(caseName);
        }
        m_cases[caseName]->tests.push_back(new Test(testName, f));
    }
    /** The vector of test casts (sets of tests) */
    std::unordered_map<std::string, TestCase *> m_cases;
    int m_retCode;

    TestRunner()
    {
        m_retCode = 0;
    }
};

#endif