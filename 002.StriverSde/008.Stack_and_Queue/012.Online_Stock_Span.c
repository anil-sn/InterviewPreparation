#include "stdio.h"
#include "stdbool.h"
#include "limits.h"
#include "math.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "sys/types.h"
#include "assert.h"

/*
    901. Online Stock Span

    Design an algorithm that collects daily price quotes for some stock and returns
    the span of that stock's price for the current day.

    The span of the stock's price today is defined as the maximum number of consecutive days
    (starting from today and going backward) for which the stock price was
    less than or equal to today's price.

    For example, if the price of a stock over the next 7 days were [100,80,60,70,60,75,85],
    then the stock spans would be [1,1,1,2,1,4,6].

    Example 1:
        Input ["StockSpanner", "next", "next", "next", "next", "next", "next", "next"]
              [[], [100], [80], [60], [70], [60], [75], [85]]
        Output
            [null, 1, 1, 1, 2, 1, 4, 6]

        Explanation
        StockSpanner stockSpanner = new StockSpanner();
        stockSpanner.next(100); // return 1
        stockSpanner.next(80);  // return 1
        stockSpanner.next(60);  // return 1
        stockSpanner.next(70);  // return 2
        stockSpanner.next(60);  // return 1
        stockSpanner.next(75);  // return 4, because the last 4 prices (including today's price of 75) were less than or equal to today's price.
        stockSpanner.next(85);  // return 6
*/

#define MAX 100

typedef struct stSpan_
{
    int price;
    int span;
} STOCK_SPAN;

STOCK_SPAN * newStockSpanNode(int price, int span)
{
    STOCK_SPAN * new = malloc(sizeof(STOCK_SPAN));
    new->price = price;
    new->span = span;
    return new;
}

typedef struct stack_ {
    STOCK_SPAN *stack[MAX];
    int top;
    int capacity;
} STACK;

STACK * initStack()
{
    STACK *stack = calloc(1, sizeof(STACK));
    stack->top = -1;
    stack->capacity = MAX;
    return stack;
}

bool isStackFull(STACK *s) {
    return (s->top >= (s->capacity - 1)) ? true : false;
}

bool isStackEmpty(STACK *s) {
    return (s->top < 0) ? true : false;
}

STOCK_SPAN* peek(STACK *s) {
    if (isStackEmpty(s)) {
        printf("Stack is Empty\r\nExiting...\r\n");
        return NULL;
    }

    return (s->stack[s->top]);
}

void push(STACK *s, STOCK_SPAN *value) {
    if (isStackFull(s)) {
        printf("Stack is full\r\nExiting...\r\n");
        return;
    }

    printf("Element %d,%d is pushed to stack \r\n", value->price, value->span);
    s->stack[++s->top] = value;
    return;
}

STOCK_SPAN * pop(STACK *s) {
    if (isStackEmpty(s)) {
        return NULL;
    }

    STOCK_SPAN *value = s->stack[s->top];
    printf("Element %d,%d is popped out of stack \r\n", value->price, value->span);
    return (s->stack[s->top--]);
}


int next(int price, STACK *s)
{
    int span = 1;
    STOCK_SPAN *stock;

    printf("Processing %d,%d \r\n", price, span);
    while (false == isStackEmpty(s)) {

        stock = peek(s);
        if (stock->price <= price) {
            (void) pop(s);
            printf("Updated to %d,%d \r\n", price, (span + stock->span));
            span += stock->span;
        } else {
            break;
        }
    }

    push(s, newStockSpanNode(price, span));
    return span;
}

void updateStockSpan(int stock[], int n, int span[])
{
    STACK *s = initStack();
    for (int i = 0; i < n; i++) {
        printf("[%d] processing price : %d \r\n", i, stock[i]);
        span[i] = next(stock[i], s);
    }

    return;
}

int main ()
{
    int stock[] = {100, 80, 60, 70, 60, 75, 85};
    int n = sizeof(stock)/sizeof(stock[0]);
    int span[n];
    memset(span, 0, sizeof(span));


    updateStockSpan(stock, n, span);
    for (int i = 0; i < n; i++) {
        printf("Stock : %d Span : %d \r\n", stock[i], span[i]);
    }
    printf("\r\n");
}