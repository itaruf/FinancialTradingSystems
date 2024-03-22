#pragma once

#include "test_utils.h"

#include <iostream>
#include <string>

/* TESTS  */
/*
TEST_SCENARIO(insert, ({
    "INSERT,1,AAPL,BUY,12.2,5"
    }), ({
        "===AAPL===",
        "12.2,5,,"
        }));

TEST_SCENARIO(simple_match_sell_is_aggressive, ({
    "INSERT,1,AAPL,BUY,12.2,5",
    "INSERT,2,AAPL,SELL,12.1,8"
    }), ({
        "AAPL,12.2,5,2,1",
        "===AAPL===",
        ",,12.1,3"
        }));

TEST_SCENARIO(simple_match_buy_is_aggressive, ({
    "INSERT,1,AAPL,SELL,12.1,8",
    "INSERT,2,AAPL,BUY,12.2,5"
    }), ({
        "AAPL,12.1,5,2,1",
        "===AAPL===",
        ",,12.1,3"
        }));

TEST_SCENARIO(multi_insert_and_multi_match, ({
    "INSERT,8,AAPL,BUY,14.235,5",
    "INSERT,6,AAPL,BUY,14.235,6",
    "INSERT,7,AAPL,BUY,14.235,12",
    "INSERT,2,AAPL,BUY,14.234,5",
    "INSERT,1,AAPL,BUY,14.23,3",
    "INSERT,5,AAPL,SELL,14.237,8",
    "INSERT,3,AAPL,SELL,14.24,9",
    "PULL,8",
    "INSERT,4,AAPL,SELL,14.234,25"
    }), ({
        "AAPL,14.235,6,4,6",
        "AAPL,14.235,12,4,7",
        "AAPL,14.234,5,4,2",
        "===AAPL===",
        "14.23,3,14.234,2",
        ",,14.237,8",
        ",,14.24,9"
        }));

TEST_SCENARIO(multi_symbol, ({
    "INSERT,1,WEBB,BUY,0.3854,5",
    "INSERT,2,TSLA,BUY,412,31",
    "INSERT,3,TSLA,BUY,410.5,27",
    "INSERT,4,AAPL,SELL,21,8",
    "INSERT,11,WEBB,SELL,0.3854,4",
    "INSERT,13,WEBB,SELL,0.3853,6"
    }), ({
        "WEBB,0.3854,4,11,1",
        "WEBB,0.3854,1,13,1",
        "===AAPL===",
        ",,21,8",
        "===TSLA===",
        "412,31,,",
        "410.5,27,,",
        "===WEBB===",
        ",,0.3853,5"
        }));

TEST_SCENARIO(amend, ({
    "INSERT,1,WEBB,BUY,45.95,5",
    "INSERT,2,WEBB,BUY,45.95,6",
    "INSERT,3,WEBB,BUY,45.95,12",
    "INSERT,4,WEBB,SELL,46,8",
    "AMEND,2,46,3",
    "INSERT,5,WEBB,SELL,45.95,1",
    "AMEND,1,45.95,3",
    "INSERT,6,WEBB,SELL,45.95,1",
    "AMEND,1,45.95,5",
    "INSERT,7,WEBB,SELL,45.95,1"
    }), ({
        "WEBB,46,3,2,4",
        "WEBB,45.95,1,5,1",
        "WEBB,45.95,1,6,1",
        "WEBB,45.95,1,7,3",
        "===WEBB===",
        "45.95,16,46,5"
        }));

*/

int main()
{
    /*TESTS*/
    /*
    test_insert();
    test_simple_match_sell_is_aggressive();
    test_simple_match_buy_is_aggressive();
    test_multi_insert_and_multi_match();
    test_multi_symbol();
    test_amend();
    */

    return 0;
}