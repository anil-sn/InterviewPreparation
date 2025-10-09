# SOLID Principles in Object-Oriented Programming

The SOLID principles are five fundamental design guidelines for object-oriented programming that help developers create software that is more maintainable, flexible, and scalable. Let's break this down step by step. First, understand that object-oriented programming revolves around concepts like classes, objects, inheritance, and polymorphism. These principles address common pitfalls in designing such systems. The acronym SOLID stands for Single Responsibility Principle, Open-Closed Principle, Liskov Substitution Principle, Interface Segregation Principle, and Dependency Inversion Principle. We will explore each one gradually, starting with the basics, providing clear explanations, addressing potential confusions, and using examples to build your understanding.

Begin with the Single Responsibility Principle, often abbreviated as SRP. This principle asserts that a class should have one, and only one, reason to change. In other words, a class should focus on a single purpose and handle only one aspect of the software's functionality. Why does this matter? When a class juggles multiple responsibilities, any change in one area can ripple through and affect unrelated parts, leading to bugs and making maintenance a nightmare. Imagine a class that both calculates employee salaries and generates payroll reports. If tax laws change, affecting salary calculations, you might inadvertently break the reporting functionality while updating the class. This violates SRP because the class has multiple reasons to change: one for salary logic and another for reporting formats.

To adhere to SRP, separate these concerns into distinct classes. For instance, create a SalaryCalculator class solely for computing salaries and a PayrollReporter class exclusively for formatting and outputting reports. This way, changes to tax laws only touch the SalaryCalculator, leaving the reporter intact. This separation not only reduces errors but also makes the code easier to test and reuse. A common point of confusion is mistaking "responsibility" for "method"—it's not about having one method per class but about ensuring the class has a cohesive, singular purpose. If you find yourself describing a class with "and" (e.g., it does this and that), it's likely violating SRP.

Moving on to the Open-Closed Principle, or OCP. This principle states that software entities, such as classes, modules, or functions, should be open for extension but closed for modification. That means you should be able to add new functionality without altering existing, well-tested code. The key here is to design systems that embrace change through extension rather than direct edits. Consider a scenario where you have a PaymentProcessor class that handles different payment types using a switch statement for credit cards, PayPal, and bank transfers. Adding a new payment type, like cryptocurrency, requires modifying the switch statement inside PaymentProcessor. This opens the door to bugs in the existing payment handling, as you're touching code that was previously stable.

Instead, to follow OCP, rely on abstractions. Define an abstract PaymentMethod interface with a process() method. Then, implement concrete classes like CreditCardPayment, PayPalPayment, and CryptoPayment, each overriding the process() method. The PaymentProcessor now depends on the PaymentMethod interface, accepting any implementation without knowing the details. When you add CryptoPayment, you extend the system by creating a new class, leaving PaymentProcessor unchanged. This approach fosters scalability and reduces risk. People often confuse OCP with never changing code at all, but it's about closing code to modification after it's in use while allowing extension through inheritance or composition. Background: This principle originated from Bertrand Meyer's work on object-oriented design, emphasizing modular reasoning.

Next is the Liskov Substitution Principle, known as LSP. It dictates that objects of a superclass should be replaceable with objects of its subclasses without affecting the correctness of the program. In essence, subclasses must honor the behavioral contract established by their parent class. Violation of LSP can lead to subtle bugs where code works with the base class but fails with derived classes. A classic example is the "square is a rectangle" dilemma. Suppose you have a Rectangle class with setWidth() and setHeight() methods. A Square class inherits from Rectangle but overrides these methods to ensure width and height remain equal. Now, if a function expects a Rectangle and calculates area after setting dimensions, passing a Square might yield unexpected results because setting width also changes height, breaking the expected behavior.

To comply with LSP, avoid such forced substitutions. Instead, design Square and Rectangle as separate classes or use a common interface only for shared behaviors that both can support without overrides that alter semantics. This ensures substitutability. Confusion often arises from mathematical relationships not translating directly to code—while a square is mathematically a rectangle, their mutable behaviors differ in programming contexts. LSP, introduced by Barbara Liskov, builds on type theory to ensure polymorphic code remains reliable.

Now, the Interface Segregation Principle, or ISP. This principle advises that clients should not be forced to depend on interfaces they do not use. Rather than having large, monolithic interfaces, break them into smaller, more specific ones tailored to client needs. A violation might involve a Worker interface with methods like work(), eat(), and sleep(). A Robot class implementing this interface must provide eat() and sleep(), even though robots don't eat or sleep, leading to empty or throwing implementations that clutter code and confuse users.

Adhering to ISP means segregating the interface into Workable (with work()), Feedable (with eat()), and Restable (with sleep()). The Robot implements only Workable, while a HumanWorker implements all three as needed. This keeps interfaces lean and relevant. The benefit is reduced coupling and easier evolution—adding a new method to one small interface doesn't force changes across unrelated clients. ISP addresses the fat interface problem common in early OOP designs, promoting cohesion.

Finally, the Dependency Inversion Principle, DIP. It states that high-level modules should not depend on low-level modules; both should depend on abstractions. Moreover, abstractions should not depend on details; details should depend on abstractions. In practice, this means avoiding tight coupling where a high-level class like NotificationService directly instantiates a low-level EmailSender. If you need to switch to SMS, you'd have to modify NotificationService.

Instead, both depend on a MessageSender abstraction. NotificationService receives a MessageSender implementation via dependency injection, remaining agnostic to the concrete type. This inversion decouples layers. A point of confusion is thinking DIP is just dependency injection—DI is a technique to achieve DIP, but the principle is broader, about abstraction levels. Originating from Robert C. Martin's work, DIP enables flexible, testable architectures.

Applying SOLID principles yields significant benefits. Your code becomes more maintainable because changes are localized. Flexibility increases as you can extend without modifying core components. Testability improves with loose coupling, allowing mocks for dependencies. However, applying them rigidly without context can lead to over-engineering—use judgment. Compared to procedural paradigms, SOLID leverages OOP features like interfaces and inheritance, which aren't native in languages like C, where you'd use function pointers or structs for similar effects.

# DRY Principle in Software Development

The DRY principle, which stands for Don't Repeat Yourself, is a cornerstone of efficient software development that emphasizes eliminating duplication in code, data, and logic. To build a deep understanding, let's start from the ground up. Duplication occurs when the same piece of knowledge or functionality appears in multiple places within a system. This can be as obvious as copy-pasted code blocks or as subtle as repeated business rules embedded in different modules. The DRY principle insists that every significant piece of information should have a single, authoritative representation. By adhering to this, you avoid the pitfalls of inconsistency and maintenance hell.

Consider why DRY is crucial. When duplication exists, any change—such as fixing a bug or updating a requirement—must be applied everywhere the duplication appears. Miss one spot, and you introduce inconsistencies or errors. This not only wastes time but also increases the risk of defects. For example, imagine a web application where validation logic for user input (like checking email formats) is replicated in both the frontend JavaScript and backend server code. If the validation rules change, you must update both, and forgetting one leads to mismatched behavior, frustrating users and complicating debugging.

To violate DRY brutally clearly: hardcoding the same constant value, like a tax rate of 0.15, in multiple functions across your codebase. When the tax rate changes to 0.18, you hunt down every instance, risking missing some and causing financial errors in calculations. This is not just inefficient; it's a recipe for disaster in large systems.

Adhering to DRY involves abstracting the repeated elements into a single source. In the tax example, define the rate in a central configuration file or constant module, then reference it everywhere. For the validation logic, create a shared library or API endpoint that both frontend and backend use. Step by step: Identify duplications during code reviews or refactoring. Extract the common parts into functions, classes, or modules. Use parameters to handle variations. In C, for instance, if you have repeated code for swapping integers in different parts of a program, define a single swap function like void swap(int *a, int *b) { int temp = *a; *a = *b; *b = temp; } // Helpful comment: Swaps two integers using a temporary variable to avoid overflow issues. Then call it wherever needed, ensuring changes to the swap logic happen in one place.

A common confusion is over-applying DRY, leading to premature abstraction. Not all similarities warrant extraction—if two pieces of code look alike but serve different conceptual purposes, forcing them together can create false couplings. Compare DRY to WET (Write Everything Twice), its antithesis, which some advocate for initial prototyping to avoid overthinking abstractions too early. However, in production code, DRY prevails for long-term health.

Background: DRY originated from Andy Hunt and Dave Thomas in "The Pragmatic Programmer," highlighting how repetition amplifies errors. Benefits include improved readability, as centralized logic is easier to comprehend, and enhanced scalability, since updates are streamlined. Brutal clarity: Ignoring DRY turns your codebase into a fragile mess where small changes cascade into major overhauls. Encourage yourself: Mastering DRY takes practice, but each application makes you a sharper developer, capable of crafting robust systems.

# KISS Principle in Software Development

The KISS principle, shorthand for Keep It Simple, Stupid, is a guiding tenet in software development that advocates for simplicity in design and implementation. To foster deep understanding, we'll unpack this gradually. Simplicity means avoiding unnecessary complexity, choosing straightforward solutions over convoluted ones. Systems built with KISS are easier to build, debug, and evolve. Complexity often creeps in from over-engineering, trying to anticipate every future need, or using advanced features without justification.

Why prioritize KISS? Complex code is harder to read, maintain, and extend. It increases bug likelihood and onboarding time for new developers. Brutal clarity: Fancy solutions might impress initially, but they often fail in practice, leading to costly rewrites. For example, violating KISS: Implementing a user authentication system with multiple layers of abstract factories, observers, and decorators when a simple username-password check with hashing suffices. This overkill obscures the core logic, making it prone to errors.

Adhering to KISS: Opt for the simplest approach that meets requirements. In a sorting task, use a built-in sort function instead of writing a custom algorithm unless performance demands it. Step by step: Define the problem clearly. Brainstorm solutions, favoring the least complex. Implement, test, and iterate if needed. In C, for a program reading files, avoid elaborate error-handling classes; use straightforward if checks like if (file == NULL) { perror("File open failed"); return 1; } // Helpful comment: Checks if file pointer is null and handles error simply.

Confusion arises when simplicity is mistaken for naivety—KISS doesn't mean dumbing down; it means elegant efficiency. Compare to YAGNI: Both combat over-engineering, but KISS focuses on current simplicity, YAGNI on avoiding unneeded features. Background: Coined by Kelly Johnson in engineering, adapted to software for practical designs.

Benefits: Faster development, fewer bugs, better performance. Encourage: Embrace simplicity; it's a skill that refines with experience, leading to masterful code.

# YAGNI Principle in Software Development

The YAGNI principle, meaning You Aren't Gonna Need It, warns against adding functionality until it's truly necessary. Building understanding step by step: Developers often speculate on future needs, adding "just in case" features. YAGNI counters this by insisting on implementing only what's required now.

Why YAGNI? Extra code bloats the system, increases maintenance, and diverts from current priorities. Brutal clarity: That flexible framework you built for hypothetical expansions? It complicates things and likely won't be used as imagined, wasting effort.

Violation example: In a blogging app, adding support for multiple languages upfront when only English is needed. This adds complexity in strings, UI, and logic unnecessarily.

Adherence: Focus on current requirements. If multilingual support becomes needed later, add it then. Step by step: Prioritize user stories. Implement minimally viable solutions. Refactor as real needs emerge.

In C, avoid pre-allocating huge arrays for potential data growth; use dynamic allocation like realloc when size increases. // Helpful comment: Resize array only when capacity is exceeded to avoid wasteful initial allocations.

Confusion: YAGNI isn't about ignoring planning; it's about deferring unproven needs. Compare to KISS: YAGNI prevents feature creep, KISS ensures simple implementations.

Background: From Extreme Programming, promotes lean development.

Benefits: Faster delivery, cleaner code. Encourage: Trust the process; adding later is easier than removing unused bloat.

# Separation of Concerns Principle in Software Development

Separation of Concerns, or SoC, is a design principle that divides a program into distinct sections, each addressing a separate concern. To explain gradually: A concern is any piece of interest, like data persistence, user interface, or business logic. Mixing them leads to tangled code.

Why SoC? It simplifies development and maintenance. Changes in one area don't affect others. Brutal clarity: Monolithic code where UI, logic, and data mix becomes a maintenance quagmire, hard to scale or debug.

Violation: A single class handling database queries, input validation, and HTML rendering. Changing the database schema forces UI tweaks.

Adherence: Use layers or modules, like MVC (Model-View-Controller). Model for data, View for UI, Controller for logic. In C, separate functions into files: data.c for structs and accessors, ui.c for display functions.

Step by step: Identify concerns. Assign to modules with clear interfaces. Minimize overlaps.

Confusion: SoC isn't microservices; it's modular thinking at any scale. Compare to SRP: SoC is broader, applying to entire systems.

Background: Coined by Edsger Dijkstra, essential for ordered thinking.

Benefits: Reusability, independent updates. Encourage: Practice modularization; it builds intuitive, resilient software.


# Principles for C Programming Language

Unlike object-oriented principles like SOLID, which rely on classes and inheritance, principles for the C language center on its procedural, low-level nature. C emphasizes explicit control, efficiency, and portability. We'll explore these step by step, providing thorough explanations, examples, and addressing confusions for deep insight. These principles help craft reliable, performant code in a language without built-in safeguards.

Start with structured programming. This paradigm organizes code into predictable control structures, ditching chaotic goto statements. In C, it's foundational. Sequential execution means statements run in order. Selection uses if-else or switch for decisions. Iteration employs for, while, do-while for repetition. Why? Unstructured code is spaghetti—hard to follow or debug. Violation: Using goto for loops, creating jumps that obscure flow. Adherence: Rewrite with while loops. Example in C: Instead of goto labels, use while (condition) { // code }. // Helpful comment: Loop continues until condition fails, ensuring clear entry and exit points.

Next, modularity. Break programs into small, self-contained functions or modules with single purposes. This echoes SRP but in procedural terms. Separation of concerns groups related functions, like file operations in file_ops.c with file_ops.h declaring prototypes. Why? Large monolithic files are overwhelming; modularity aids compilation and reuse. Confusion: Modularity isn't just splitting files—ensure logical cohesion. Example: A main.c calls functions from math_ops.c, like int add(int a, int b) { return a + b; } // Helpful comment: Simple addition to avoid inline calculations elsewhere.

Low-level memory management is key in C, offering direct control absent in higher languages. Pointers access addresses for arrays or dynamics. Dynamic allocation uses malloc, calloc, realloc, free. Why? Efficiency in resource-constrained environments. Violation: Forgetting free leads to leaks. Adherence: Always pair allocations with frees. Example: char *str = malloc(10); // Use str... free(str); // Helpful comment: Releases memory to prevent leaks; check malloc return for NULL.

Abstraction hides complexity. Structs bundle data: struct Point { int x; int y; }; Functions abstract operations. Why? Manages complexity without OOP. Confusion: C's abstraction is manual, via headers. Example: Declare struct in .h, define functions operating on it.

Portability ensures code runs across platforms. Use standard library, avoid extensions. Why? C's UNIX roots aimed for this. Example: Use printf over non-standard funcs.

Defensive programming prevents bugs. Check returns, initialize vars, manage buffers. Why? C's lack of bounds checking invites overflows. Example: char buf[10]; strncpy(buf, input, 9); buf[9] = '\0'; // Helpful comment: Prevents overflow by limiting copy and null-terminating.

These principles make C code robust. Benefits: Performance, control. Brutal clarity: Ignore them, and your code crashes unpredictably. Encourage: Build gradually; mastery comes with practice.

# C Coding Guidelines and Best Practices

This document outlines coding guidelines and best practices for writing robust, maintainable, and efficient C code. It is tailored for projects requiring high reliability, such as multi-threaded applications (e.g., modx), and emphasizes simplicity, readability, and debuggability. The guidelines include thumb rules, specific coding standards (e.g., CODE-GUIDE), and AStyle configuration for consistent formatting. Example code is provided to illustrate compliance.

## Thumb Rules

1. **Keep The Code Small & Simple**: Prioritize concise code to reduce complexity and improve maintainability. Avoid over-engineering.
2. **Focus on Code Readability & Debuggability**: Write code that is easy to understand and trace during debugging.
3. **Design Efficient Data Structures**: Choose data structures with optimal time complexity:
   - Constant: Θ(1)
   - Logarithmic: Θ(log N)
   - Linear: Θ(N)
   - Polynomial: Θ(N²)
   - Exponential: Θ(2^N)
   - Factorial: Θ(N!)
4. **One Function, One Task**: Each function should perform a single task well, handling all error cases at the beginning before executing the main logic.
5. **Error Handling**: Every function must return an error code, and the caller must handle it. Log errors with input, output, and remarks at DEBUG level.
6. **Function Size and Complexity**: Functions should not exceed 200 lines and must have a cyclomatic complexity below 20. Avoid multiple conditions in a single `if` statement.
7. **Function Naming**: Use the syntax `<Component><Subcomponent><DS-name><operation>` (e.g., `module_x_pkt_handle_uplink`). Names should be descriptive and in snake_case.
8. **Variable Declarations**: Define and initialize all local variables at the function’s start. Global variables start with `g_module_x_` and are grouped in a single structure.
9. **Parameter Limits**: Functions should have at most 5 parameters, preferably 3. Pass pointers instead of structures.
10. **No Duplicate Logic**: Use small, reusable functions to avoid code duplication.
11. **No Nested `if` Conditions**: Flatten logic to improve readability. Check failure cases early in functions and loops, exiting or continuing as needed.
12. **No Magic Numbers**: Use MACROs for numerical values, clearly documenting their purpose.
13. **Logging**: Logs must be grep-friendly, including entry/exit traces if needed, with ERROR and DEBUG levels.
14. **Indentation**: Use 4 spaces for indentation, not tabs, to ensure consistency.
15. **Comments**: Document logic or intent, not obvious code. Use Doxygen-style comments for functions.
16. **No Type Casting**: Use strict types to avoid runtime issues.
17. **Meaningful Variable Names**: Use descriptive names to clarify purpose (e.g., `packet_buffer` vs. `buf`).
18. **Unified Logic Flow**: Converge multiple entry points into a core function for common logic, then diverge for specific cases (high fan-in, high fan-out).
19. **File Organization**: Group operations for a specific data structure or logical unit in dedicated `.c` and `.h` files (e.g., `module_x_pkt.c`, `module_x_pkt.h`).
20. **No Global Variables in Multi-Threaded Code**: Group globals in a single structure to avoid concurrency issues.

## AStyle Configuration

AStyle ensures consistent code formatting. Create a `~/.astylerc` file with the following configuration:

```
/* ~/.astylerc */
# General Formatting Style
style=kr                          # K&R style: opening braces on the same line
mode=c                            # Format C source files
lineend=linux                     # Use \n line endings
preserve-date                     # Keep file modification dates

# Indentation
indent=spaces=4                   # Use 4 spaces for indentation
indent-classes                    # Indent class blocks
indent-switches                   # Indent switch statements
indent-cases                      # Indent case statements
indent-preproc-block              # Indent preprocessor blocks
indent-preproc-define             # Indent #define statements
indent-col1-comments              # Place comments in column 1
indent-continuation=4             # Indent continuation lines by 4 spaces
indent-namespaces                 # Indent nested namespaces

# Line Length and Breaking
max-code-length=120               # Break lines longer than 120 characters
break-after-logical               # Break after logical operators
break-one-line-headers            # Break headers like if/for
break-blocks=all                  # Break all block structures

# Spacing and Padding
pad-oper                          # Spaces around operators
pad-header                        # Space after control headers (e.g., if)
pad-paren-out                     # Space outside parentheses
unpad-paren                       # Remove space inside parentheses
pad-comma                         # Space after commas

# Alignment
align-pointer=name                # Align pointers with variable names
align-reference=name              # Align references with variable names

# Miscellaneous
convert-tabs                      # Convert tabs to spaces
add-braces                        # Add braces to single-line statements
keep-one-line-blocks              # Keep single-line blocks
keep-one-line-statements          # Keep single-line statements
delete-empty-lines                # Remove excess empty lines
squeeze-lines=2                   # Keep up to 2 blank lines

# Output Options
formatted                         # Format the file
suffix=.orig                      # Keep original files with .orig suffix
verbose                           # Provide verbose output
min-conditional-indent=0          # Minimal indent for conditionals
```

Run AStyle before committing:
```bash
astyle --options=~/.astylerc --recursive "*.c" --recursive "*.h" --exclude=src/smdesigner --exclude=src/control-graphs
```

## Coding Guidelines

### Preprocessor

- **CODE-GUIDE-PRE-PROC-01**: Use `##` or `#` operators only alone, not nested or mixed.
  ```c
  /* Compliant */
  #define CONCAT(x, y) x ## y
  uint32_t ab = 32U;
  printf("%d\n", CONCAT(a, b)); /* Prints 32 */
  
  /* Non-compliant */
  #define CONCAT(x, y, z) x ## y ## z
  uint32_t abc = 32U;
  printf("%d\n", CONCAT(a, b, c));
  ```

- **CODE-GUIDE-PRE-PROC-02**: Prefer inline functions over function-like macros.
  ```c
  /* Compliant */
  static inline uint32_t add(uint32_t a, uint32_t b) {
      return a + b;
  }
  
  /* Non-compliant */
  #define ADD(a, b) ((a) + (b))
  ```

- **CODE-GUIDE-PRE-PROC-03**: Use include guards to prevent multiple inclusions.
  ```c
  /* Compliant: module_x_pkt.h */
  #ifndef MODULE_X_PKT_H
  #define MODULE_X_PKT_H
  
  uint32_t module_x_pkt_process(struct pkt_obj *pkt);
  
  #endif /* MODULE_X_PKT_H */
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt);
  ```

- **CODE-GUIDE-PRE-PROC-04**: Use parentheses for macro parameters.
  ```c
  /* Compliant */
  #define NEGATE(x) -(x)
  
  /* Non-compliant */
  #define NEGATE(x) -x
  ```

### Compilation Units

- **CODE-GUIDE-COMP-UNIT-01**: One assignment per line.
  ```c
  /* Compliant */
  uint32_t a = 0U;
  uint32_t b = 0U;
  
  /* Non-compliant */
  uint32_t a = b = 0U;
  ```

- **CODE-GUIDE-COMP-UNIT-02**: One return statement per function.
  ```c
  /* Compliant */
  int32_t module_x_pkt_validate(const struct pkt_obj *pkt) {
      int32_t ret = 0;
      if (pkt == NULL) {
          ret = -EINVAL;
      }
      return ret;
  }
  
  /* Non-compliant */
  int32_t module_x_pkt_validate(const struct pkt_obj *pkt) {
      if (pkt == NULL) {
          return -EINVAL;
      }
      return 0;
  }
  ```

- **CODE-GUIDE-COMP-UNIT-03**: All code must be reachable.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_count(void) {
      uint32_t count = 0U;
      printf("Count: %u\n", count);
      return count;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_count(void) {
      uint32_t count = 0U;
      return count;
      printf("Count: %u\n", count); /* Unreachable */
  }
  ```

- **CODE-GUIDE-COMP-UNIT-04**: Cyclomatic complexity < 20.
  ```c
  /* Compliant */
  bool module_x_pkt_is_valid_type(uint32_t type) {
      bool valid = false;
      if ((type & 0x1U) == 0U) {
          valid = true;
      }
      return valid;
  }
  
  uint32_t module_x_pkt_process_type(uint32_t type) {
      uint32_t ret = 0U;
      if (type >= 20U) {
          ret = 20U;
      } else if (module_x_pkt_is_valid_type(type)) {
          ret = 10U;
      }
      return ret;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process_type(uint32_t type) {
      uint32_t ret = 0U;
      if (type >= 20U) {
          ret = 20U;
      }
      if (type == 0U || type == 2U || type == 4U || type == 6U || type == 8U ||
          type == 10U || type == 12U || type == 14U || type == 16U || type == 18U) {
          ret = 10U;
      }
      return ret;
  }
  ```

### Declarations and Initialization

- **CODE-GUIDE-DECL-INIT-01**: Use variables after initialization.
  ```c
  /* Compliant */
  uint32_t a = 0U;
  uint32_t b = a;
  
  /* Non-compliant */
  uint32_t a, b;
  b = a; /* a is uninitialized */
  ```

- **CODE-GUIDE-DECL-INIT-02**: Call functions after declaration.
  ```c
  /* Compliant */
  static void module_x_log_error(void) {
      /* Log error */
  }
  
  static void module_x_pkt_init(void) {
      module_x_log_error();
  }
  
  /* Non-compliant */
  static void module_x_pkt_init(void) {
      module_x_log_error(); /* Not yet declared */
  }
  
  static void module_x_log_error(void) {
      /* Log error */
  }
  ```

- **CODE-GUIDE-DECL-INIT-03**: Do not skip initialization.
  ```c
  /* Compliant */
  uint32_t count = 0U;
  goto cleanup;
  count += 10U;
  cleanup:
      return count;
  
  /* Non-compliant */
  uint32_t count;
  goto cleanup;
  count = 0U;
  cleanup:
      return count;
  ```

- **CODE-GUIDE-DECL-INIT-04**: Initialize structs with brackets.
  ```c
  /* Compliant */
  struct module_x_pkt {
      uint32_t id;
      uint32_t size;
  };
  struct module_x_pkt pkt = {0U, 64U};
  
  /* Non-compliant */
  struct module_x_pkt pkt = {0U, 64U}; /* Missing braces */
  ```

- **CODE-GUIDE-DECL-INIT-05**: Specify array sizes explicitly.
  ```c
  /* Compliant */
  uint32_t buffer[32] = {0U};
  
  /* Non-compliant */
  uint32_t buffer[] = {0U};
  ```

- **CODE-GUIDE-DECL-INIT-06**: Declare globals only once.
  ```c
  /* Compliant: module_x_pkt.h */
  extern uint32_t g_module_x_packet_count;
  
  /* module_x_pkt.c */
  uint32_t g_module_x_packet_count = 0U;
  
  /* Non-compliant */
  uint32_t g_module_x_packet_count;
  uint32_t g_module_x_packet_count = 0U; /* Duplicate */
  ```

- **CODE-GUIDE-DECL-INIT-07**: Fully initialize arrays.
  ```c
  /* Compliant */
  uint32_t buffer[5] = {0U, 1U, 2U, 3U, 4U};
  
  /* Non-compliant */
  uint32_t buffer[5] = {0U, 1U}; /* Partial initialization */
  ```

- **CODE-GUIDE-DECL-INIT-08**: Use constants for array sizes.
  ```c
  /* Compliant */
  #define BUFFER_SIZE 10U
  uint32_t buffer[BUFFER_SIZE];
  
  /* Non-compliant */
  uint32_t size = 10U;
  uint32_t buffer[size];
  ```

### Functions

- **CODE-GUIDE-FUNC-01**: Non-void functions must have a return statement.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_get_id(const struct pkt_obj *pkt) {
      return pkt->id;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_get_id(const struct pkt_obj *pkt) {
      /* Missing return */
  }
  ```

- **CODE-GUIDE-FUNC-02**: Non-void functions must return a value.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_get_id(const struct pkt_obj *pkt) {
      return pkt->id;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_get_id(const struct pkt_obj *pkt) {
      return; /* Empty return */
  }
  ```

- **CODE-GUIDE-FUNC-03**: Return values on all paths.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_get_size(const struct pkt_obj *pkt) {
      if (pkt == NULL) {
          return 0U;
      }
      return pkt->size;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_get_size(const struct pkt_obj *pkt) {
      if (pkt != NULL) {
          return pkt->size;
      } /* Missing return for NULL case */
  }
  ```

- **CODE-GUIDE-FUNC-04**: Do not use void function returns.
  ```c
  /* Compliant */
  void module_x_log_init(void) {
      /* Initialize logging */
  }
  
  void module_x_pkt_init(void) {
      module_x_log_init();
  }
  
  /* Non-compliant */
  void module_x_log_init(void) {
      /* Initialize logging */
  }
  
  void module_x_pkt_init(void) {
      uint32_t ret = module_x_log_init(); /* Void return used */
  }
  ```

- **CODE-GUIDE-FUNC-05**: Do not reassign pointer parameters.
  ```c
  /* Compliant */
  void module_x_pkt_process(const struct pkt_obj *pkt) {
      const struct pkt_obj *local_pkt = pkt;
      /* Use local_pkt */
  }
  
  /* Non-compliant */
  void module_x_pkt_process(const struct pkt_obj *pkt) {
      pkt++; /* Reassigns parameter */
  }
  ```

- **CODE-GUIDE-FUNC-06**: Do not modify value parameters directly.
  ```c
  /* Compliant */
  void module_x_pkt_increment(uint32_t count) {
      uint32_t local_count = count;
      local_count++;
  }
  
  /* Non-compliant */
  void module_x_pkt_increment(uint32_t count) {
      count++; /* Modifies parameter */
  }
  ```

- **CODE-GUIDE-FUNC-07**: Declare non-static functions in headers.
  ```c
  /* Compliant: module_x_pkt.h */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt);
  
  /* module_x_pkt.c */
  #include "module_x_pkt.h"
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U; /* No header declaration */
  }
  ```

- **CODE-GUIDE-FUNC-08**: Use static functions within their file.
  ```c
  /* Compliant */
  static void module_x_log_internal(void) {
      /* Log */
  }
  
  void module_x_pkt_init(void) {
      module_x_log_internal();
  }
  
  /* Non-compliant */
  static void module_x_log_internal(void) {
      /* Log */
  } /* Never used */
  ```

- **CODE-GUIDE-FUNC-09**: Consistent parameter names.
  ```c
  /* Compliant: module_x_pkt.h */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt);
  
  /* module_x_pkt.c */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *packet) {
      return 0U; /* Name mismatch */
  }
  ```

- **CODE-GUIDE-FUNC-10**: Consistent parameter types.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt);
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(void *pkt);
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  ```

- **CODE-GUIDE-FUNC-11**: Consistent return types.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt);
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  
  /* Non-compliant */
  int32_t module_x_pkt_process(struct pkt_obj *pkt);
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  ```

- **CODE-GUIDE-FUNC-12**: Avoid dynamic allocation for stack-scoped variables.
  ```c
  /* Compliant */
  uint32_t buffer[32];
  
  /* Non-compliant */
  uint32_t *buffer = malloc(32U * sizeof(uint32_t));
  ```

- **CODE-GUIDE-FUNC-13**: All declared functions must be defined.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt);
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt); /* No definition */
  ```

- **CODE-GUIDE-FUNC-14**: All defined functions must be used.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U;
  }
  
  void main(void) {
      module_x_pkt_process(NULL);
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0U; /* Never called */
  }
  ```

- **CODE-GUIDE-FUNC-15**: Do not return pointers to local objects.
  ```c
  /* Compliant */
  void module_x_pkt_fill(struct pkt_obj *pkt) {
      pkt->id = 1U;
  }
  
  /* Non-compliant */
  struct pkt_obj *module_x_pkt_create(void) {
      struct pkt_obj pkt;
      return &pkt; /* Local object */
  }
  ```

- **CODE-GUIDE-FUNC-16**: No mixed C and assembly in a single function.
  ```c
  /* Compliant */
  void asm_hlt(void) {
      asm volatile ("hlt");
  }
  
  void module_x_pkt_halt(void) {
      asm_hlt();
  }
  
  /* Non-compliant */
  void module_x_pkt_halt(void) {
      asm volatile ("hlt"); /* Mixed with C */
  }
  ```

- **CODE-GUIDE-FUNC-17**: Use or discard non-void function returns.
  ```c
  /* Compliant */
  int32_t module_x_pkt_validate(struct pkt_obj *pkt) {
      return pkt == NULL ? -EINVAL : 0;
  }
  
  void main(void) {
      int32_t ret = module_x_pkt_validate(NULL);
      if (ret != 0) {
          module_x_log(ERROR, "Validation failed");
      }
  }
  
  /* Non-compliant */
  void main(void) {
      module_x_pkt_validate(NULL); /* Return ignored */
  }
  ```

- **CODE-GUIDE-FUNC-18**: Validate array sizes for function parameters.
  ```c
  /* Compliant */
  void module_x_pkt_copy(uint32_t dst[16], const uint32_t src[16]) {
      memcpy(dst, src, 16U * sizeof(uint32_t));
  }
  
  /* Non-compliant */
  void module_x_pkt_copy(uint32_t dst[16], const uint32_t src[32]) {
      memcpy(dst, src, 32U * sizeof(uint32_t)); /* Overflow */
  }
  ```

- **CODE-GUIDE-FUNC-19**: Avoid recursion.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_factorial(uint32_t n) {
      uint32_t result = 1U;
      for (uint32_t i = 1U; i <= n; i++) {
          result *= i;
      }
      return result;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_factorial(uint32_t n) {
      if (n == 0U) {
          return 1U;
      }
      return n * module_x_pkt_factorial(n - 1U);
  }
  ```

- **CODE-GUIDE-FUNC-20**: Limit functions to 5 parameters.
  ```c
  /* Compliant */
  void module_x_pkt_process(struct pkt_obj *pkt, void *ctx, uint32_t vrf_id) {
      /* Process */
  }
  
  /* Non-compliant */
  void module_x_pkt_process(struct pkt_obj *pkt, void *ctx, uint32_t vrf_id,
                        uint32_t type, uint32_t size, uint32_t extra) {
      /* Too many parameters */
  }
  ```

- **CODE-GUIDE-FUNC-21**: Pass structs by reference, not value.
  ```c
  /* Compliant */
  void module_x_pkt_fill(struct pkt_obj *pkt) {
      pkt->id = 1U;
  }
  
  /* Non-compliant */
  void module_x_pkt_fill(struct pkt_obj pkt) {
      pkt.id = 1U;
  }
  ```

### Statements

- **CODE-GUIDE-STATEMENT-01**: No constant conditions in loops or selections.
  ```c
  /* Compliant */
  void module_x_pkt_process(uint32_t type) {
      if (type != 0U) {
          /* Process */
      }
  }
  
  /* Non-compliant */
  void module_x_pkt_process(uint32_t type) {
      if (false) {
          /* Unreachable */
      }
  }
  ```

- **CODE-GUIDE-STATEMENT-02**: Enclose loop bodies in braces.
  ```c
  /* Compliant */
  for (uint32_t i = 0U; i < 5U; i++) {
      printf("Index: %u\n", i);
  }
  
  /* Non-compliant */
  for (uint32_t i = 0U; i < 5U; i++)
      printf("Index: %u\n", i);
  ```

- **CODE-GUIDE-STATEMENT-03**: No infinite loops.
  ```c
  /* Compliant */
  uint32_t count = 10U;
  while (count > 5U) {
      count--;
  }
  
  /* Non-compliant */
  while (true) {
      /* No exit */
  }
  ```

- **CODE-GUIDE-STATEMENT-04**: Non-empty `else` after `else if`.
  ```c
  /* Compliant */
  uint32_t type, result;
  if (type < 10U) {
      result = 10U;
  } else if (type < 20U) {
      result = 20U;
  } else {
      result = 30U;
  }
  
  /* Non-compliant */
  if (type < 10U) {
      result = 10U;
  } else if (type < 20U) {
      result = 20U;
  } else {
      /* Empty */
  }
  ```

- **CODE-GUIDE-STATEMENT-05**: Include `default` in `switch` statements.
  ```c
  /* Compliant */
  switch (type) {
  case 1:
      break;
  default:
      break;
  }
  
  /* Non-compliant */
  switch (type) {
  case 1:
      break;
  }
  ```

- **CODE-GUIDE-STATEMENT-06**: Terminate switch clauses with `break`.
  ```c
  /* Compliant */
  switch (type) {
  case 1:
      break;
  default:
      break;
  }
  
  /* Non-compliant */
  switch (type) {
  case 1:
      /* Fallthrough */
  default:
      break;
  }
  ```

- **CODE-GUIDE-STATEMENT-07**: Do not modify loop counters in the body.
  ```c
  /* Compliant */
  for (uint32_t i = 0U; i < 5U; i++) {
      printf("Index: %u\n", i);
  }
  
  /* Non-compliant */
  for (uint32_t i = 0U; i < 5U; i++) {
      i++; /* Modifies counter */
  }
  ```

- **CODE-GUIDE-STATEMENT-08**: Use `goto` only for forward cleanup.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      uint32_t ret = 0U;
      if (pkt == NULL) {
          ret = -EINVAL;
          goto cleanup;
      }
      /* Process */
  cleanup:
      return ret;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(struct pkt_obj *pkt) {
      uint32_t ret = 0U;
  retry:
      if (pkt == NULL) {
          goto retry; /* Backward jump */
      }
      return ret;
  }
  ```

### Expressions

- **CODE-GUIDE-EXPRESSION-01**: Simple `for` loop initialization.
  ```c
  /* Compliant */
  for (uint32_t i = 0U; i < 5U; i++) {
      /* Loop */
  }
  
  /* Non-compliant */
  uint32_t count = 0U;
  for (uint32_t i = 0U, count = 10U; i < 5U; i++) {
      /* Loop */
  }
  ```

- **CODE-GUIDE-EXPRESSION-02**: Non-empty `for` loop condition.
  ```c
  /* Compliant */
  for (uint32_t i = 0U; i < 5U; i++) {
      /* Loop */
  }
  
  /* Non-compliant */
  for (uint32_t i = 0U; ; i++) {
      if (i > 4U) break;
  }
  ```

- **CODE-GUIDE-EXPRESSION-03**: Simple `for` loop increment.
  ```c
  /* Compliant */
  for (uint32_t i = 0U; i < 5U; i++) {
      /* Loop */
  }
  
  /* Non-compliant */
  uint32_t count = 0U;
  for (uint32_t i = 0U; i < 5U; i++, count++) {
      /* Loop */
  }
  ```

- **CODE-GUIDE-EXPRESSION-04**: Evaluation order independence.
  ```c
  /* Compliant */
  uint32_t a = 0U;
  uint32_t b = a + 1U;
  
  /* Non-compliant */
  uint32_t a = 0U;
  uint32_t b = a++ + 1U; /* Depends on evaluation order */
  ```

- **CODE-GUIDE-EXPRESSION-05**: Explicit operator precedence with parentheses.
  ```c
  /* Compliant */
  uint32_t result = a * (b + c);
  
  /* Non-compliant */
  uint32_t result = a * b + c; /* Ambiguous precedence */
  ```

- **CODE-GUIDE-EXPRESSION-06**: Prevent overflows.
  ```c
  /* Compliant */
  uint8_t value = 255U;
  
  /* Non-compliant */
  uint8_t value = 255U + 1U; /* Overflow */
  ```

- **CODE-GUIDE-EXPRESSION-07**: No negation on unsigned types.
  ```c
  /* Compliant */
  int32_t value = -10;
  
  /* Non-compliant */
  uint32_t value = -10U;
  ```

- **CODE-GUIDE-EXPRESSION-08**: No pointers to objects with longer lifetimes.
  ```c
  /* Compliant */
  void module_x_pkt_process(void) {
      uint32_t local = 0U;
      uint32_t *p_local = &local;
  }
  
  /* Non-compliant */
  uint32_t *g_module_x_ptr;
  void module_x_pkt_process(void) {
      uint32_t local = 0U;
      g_module_x_ptr = &local; /* Local lifetime */
  }
  ```

- **CODE-GUIDE-EXPRESSION-09**: Avoid `sizeof` on array parameters.
  ```c
  /* Compliant */
  #define PKT_SIZE 32U
  void module_x_pkt_process(uint32_t pkt[PKT_SIZE]) {
      uint32_t size = PKT_SIZE * sizeof(uint32_t);
  }
  
  /* Non-compliant */
  void module_x_pkt_process(uint32_t pkt[32]) {
      uint32_t size = sizeof(pkt); /* Returns pointer size */
  }
  ```

- **CODE-GUIDE-EXPRESSION-10**: Ensure `strlen` arguments are null-terminated.
  ```c
  /* Compliant */
  char buffer[3] = {'a', 'b', '\0'};
  size_t len = strlen(buffer);
  
  /* Non-compliant */
  char buffer[2] = {'a', 'b'};
  size_t len = strlen(buffer); /* No null terminator */
  ```

- **CODE-GUIDE-EXPRESSION-11**: No string copying with memory overlap.
  ```c
  /* Compliant */
  char src[] = "test";
  char dst[10];
  strncpy(dst, src, 5U);
  
  /* Non-compliant */
  char src[] = "test";
  char *dst = src + 1;
  strncpy(dst, src, 5U); /* Overlap */
  ```

- **CODE-GUIDE-EXPRESSION-12**: No `memcpy` with overlapping memory.
  ```c
  /* Compliant */
  char src[] = "test";
  char dst[10];
  memcpy(dst, src, 5U);
  
  /* Non-compliant */
  char src[10];
  char *dst = src + 1;
  memcpy(dst, src, 5U); /* Overlap */
  ```

- **CODE-GUIDE-EXPRESSION-13**: No assignments with overlapping storage.
  ```c
  /* Compliant */
  union module_x_data {
      uint8_t bytes[4];
      uint16_t words[2];
  };
  union module_x_data data;
  data.words[0] = 0U;
  data.bytes[3] = (uint8_t)data.words[0];
  
  /* Non-compliant */
  data.bytes[0] = (uint8_t)data.words[0]; /* Overlap */
  ```

- **CODE-GUIDE-EXPRESSION-14**: Ensure sufficient space for operations.
  ```c
  /* Compliant */
  uint32_t src[16];
  uint32_t dst[16];
  memcpy(dst, src, 16U * sizeof(uint32_t));
  
  /* Non-compliant */
  uint32_t dst[8];
  memcpy(dst, src, 16U * sizeof(uint32_t)); /* Overflow */
  ```

- **CODE-GUIDE-EXPRESSION-15**: Valid sizes for `memcpy`/`memset`.
  ```c
  /* Compliant */
  #define BUFFER_SIZE 16U
  uint32_t buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE * sizeof(uint32_t));
  
  /* Non-compliant */
  memset(buffer, 0, BUFFER_SIZE * 2U * sizeof(uint32_t)); /* Invalid size */
  ```

- **CODE-GUIDE-EXPRESSION-16**: Check for zero denominators.
  ```c
  /* Compliant */
  uint32_t numerator = 10U;
  uint32_t denominator = 0U;
  if (denominator != 0U) {
      uint32_t result = numerator / denominator;
  }
  
  /* Non-compliant */
  uint32_t result = numerator / denominator; /* Possible zero division */
  ```

- **CODE-GUIDE-EXPRESSION-17**: Check pointers before dereferencing.
  ```c
  /* Compliant */
  uint32_t *ptr = NULL;
  if (ptr != NULL) {
      uint32_t value = *ptr;
  }
  
  /* Non-compliant */
  uint32_t value = *ptr; /* NULL dereference */
  ```

- **CODE-GUIDE-EXPRESSION-18**: Do not modify string literals.
  ```c
  /* Compliant */
  const char *str = "test";
  
  /* Non-compliant */
  char *str = "test";
  str[0] = 'T';
  ```

- **CODE-GUIDE-EXPRESSION-19**: Restrict `++`/`--` usage.
  ```c
  /* Compliant */
  uint32_t count = 0U;
  count++;
  
  /* Non-compliant */
  uint32_t result = count++;
  ```

- **CODE-GUIDE-EXPRESSION-20**: Ensure in-bounds array indexing.
  ```c
  /* Compliant */
  uint32_t buffer[5];
  buffer[0] = 1U;
  
  /* Non-compliant */
  buffer[10] = 1U; /* Out of bounds */
  ```

- **CODE-GUIDE-EXPRESSION-21**: Avoid comma operator.
  ```c
  /* Compliant */
  uint32_t a = 0U;
  uint32_t b = 0U;
  a++;
  b++;
  
  /* Non-compliant */
  a++, b++;
  ```

- **CODE-GUIDE-EXPRESSION-22**: No magic numbers; use MACROs.
  ```c
  /* Compliant */
  #define MAX_PKT_SIZE 1500U
  uint32_t size = MAX_PKT_SIZE;
  
  /* Non-compliant */
  uint32_t size = 1500U;
  ```

- **CODE-GUIDE-EXPRESSION-23**: Restrict pointer arithmetic.
  ```c
  /* Compliant */
  #define BUFFER_SIZE 32U
  uint32_t buffer[BUFFER_SIZE];
  for (uint32_t i = 0U; i < BUFFER_SIZE; i++) {
      buffer[i] = i;
  }
  
  /* Non-compliant */
  uint32_t *ptr = buffer;
  for (uint32_t i = 0U; i < BUFFER_SIZE; i++) {
      *ptr = i;
      ptr++;
  }
  ```

### Types

- **CODE-GUIDE-TYPE-01**: Return values match declared types.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_get_id(const struct pkt_obj *pkt) {
      return pkt->id;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_get_id(const struct pkt_obj *pkt) {
      return -1; /* Signed return */
  }
  ```

- **CODE-GUIDE-TYPE-02**: Use unsigned types for bit operations.
  ```c
  /* Compliant */
  uint32_t value = 0xFFU;
  uint32_t mask = 0x0FU;
  value &= mask;
  
  /* Non-compliant */
  int32_t mask = -1;
  value &= mask;
  ```

- **CODE-GUIDE-TYPE-03**: No mixed Boolean/integer usage.
  ```c
  /* Compliant */
  bool valid = true;
  uint32_t result = valid ? 1U : 0U;
  
  /* Non-compliant */
  uint32_t valid = 1U;
  uint32_t result = valid ? 1U : 0U;
  ```

- **CODE-GUIDE-TYPE-04**: No arithmetic on enums.
  ```c
  /* Compliant */
  enum e_module_x_status { MODULE_X_OK, MODULE_X_ERROR };
  enum e_module_x_status status = MODULE_X_OK;
  
  /* Non-compliant */
  enum e_module_x_status status = MODULE_X_OK + 1;
  ```

- **CODE-GUIDE-TYPE-05**: No `static` in array indices.
  ```c
  /* Compliant */
  uint32_t buffer[2] = {0U, 1U};
  uint32_t value = buffer[1];
  
  /* Non-compliant */
  uint32_t value = buffer[static 1];
  ```

- **CODE-GUIDE-TYPE-06**: Use `const` for non-modified pointers.
  ```c
  /* Compliant */
  void module_x_pkt_read(const uint32_t *data) {
      printf("%u\n", *data);
  }
  
  /* Non-compliant */
  void module_x_pkt_read(uint32_t *data) {
      printf("%u\n", *data);
  }
  ```

- **CODE-GUIDE-TYPE-07**: Consistent types in ternary operations.
  ```c
  /* Compliant */
  uint32_t result = condition ? 1U : 0U;
  
  /* Non-compliant */
  uint32_t result = condition ? -1 : 0U;
  ```

- **CODE-GUIDE-TYPE-08**: Consistent struct field types.
  ```c
  /* Compliant */
  struct module_x_pkt {
      uint32_t id;
  };
  struct module_x_pkt pkt = {0U};
  
  /* Non-compliant */
  struct module_x_pkt pkt = {-1};
  ```

- **CODE-GUIDE-TYPE-09**: Consistent types in `switch` statements.
  ```c
  /* Compliant */
  enum e_module_x_type { TYPE_A, TYPE_B };
  enum e_module_x_type type;
  switch (type) {
  case TYPE_A:
      break;
  default:
      break;
  }
  
  /* Non-compliant */
  switch (type) {
  case 0:
      break;
  default:
      break;
  }
  ```

- **CODE-GUIDE-TYPE-10**: Do not discard `const` in casts.
  ```c
  /* Compliant */
  const uint32_t *src;
  const uint32_t *dst = src;
  
  /* Non-compliant */
  uint32_t *dst = (uint32_t *)src;
  ```

- **CODE-GUIDE-TYPE-11**: Use `static` for file-local variables.
  ```c
  /* Compliant */
  static uint32_t g_module_x_counter = 0U;
  
  /* Non-compliant */
  uint32_t g_module_x_counter = 0U;
  ```

- **CODE-GUIDE-TYPE-12**: Explicit type conversions.
  ```c
  /* Compliant */
  uint32_t value = (uint32_t)64UL;
  
  /* Non-compliant */
  uint32_t value = 64UL;
  ```

- **CODE-GUIDE-TYPE-13**: Cast operands, not expressions.
  ```c
  /* Compliant */
  uint64_t result = (uint64_t)a + (uint64_t)b;
  
  /* Non-compliant */
  uint64_t result = (uint64_t)(a + b);
  ```

- **CODE-GUIDE-TYPE-14**: Cast complex expressions to integers only.
  ```c
  /* Compliant */
  uint32_t value = 0x61U + 1U;
  char c = (char)value;
  
  /* Non-compliant */
  char c = (char)(0x61U + 1U);
  ```

- **CODE-GUIDE-TYPE-15**: Use characters, not integers, where expected.
  ```c
  /* Compliant */
  char c;
  switch (c) {
  case 'a':
      break;
  default:
      break;
  }
  
  /* Non-compliant */
  switch (c) {
  case 0x61:
      break;
  default:
      break;
  }
  ```

- **CODE-GUIDE-TYPE-16**: No casting pointers to other types.
  ```c
  /* Compliant */
  uint32_t *ptr;
  uint32_t value = *ptr;
  
  /* Non-compliant */
  uint32_t value = (uint32_t)ptr;
  ```

- **CODE-GUIDE-TYPE-17**: No casting non-pointers to pointers.
  ```c
  /* Compliant */
  uint32_t value = 0U;
  uint32_t *ptr = &value;
  
  /* Non-compliant */
  uint32_t *ptr = (uint32_t *)value;
  ```

- **CODE-GUIDE-TYPE-18**: Use all `typedef` types.
  ```c
  /* Compliant */
  typedef uint32_t pkt_id_t;
  pkt_id_t id = 0U;
  
  /* Non-compliant */
  typedef uint32_t pkt_id_t; /* Unused */
  ```

- **CODE-GUIDE-TYPE-19**: Index only array types.
  ```c
  /* Compliant */
  uint32_t buffer[5];
  uint32_t value = buffer[0];
  
  /* Non-compliant */
  uint32_t *ptr;
  uint32_t value = ptr[0];
  ```

- **CODE-GUIDE-TYPE-20**: Match actual and formal parameter types.
  ```c
  /* Compliant */
  void module_x_pkt_process(uint32_t id) {}
  uint32_t id = 0U;
  module_x_pkt_process(id);
  
  /* Non-compliant */
  uint64_t id = 0UL;
  module_x_pkt_process(id);
  ```

- **CODE-GUIDE-TYPE-21**: Use signed/unsigned integers or `bool` for bit-fields.
  ```c
  /* Compliant */
  struct module_x_pkt {
      uint8_t flags : 3;
  };
  
  /* Non-compliant */
  struct module_x_pkt {
      int flags : 3;
  };
  ```

- **CODE-GUIDE-TYPE-22**: No casting pointers to different object types.
  ```c
  /* Compliant */
  struct module_x_pkt *pkt;
  uint32_t *id = pkt->id_ptr;
  
  /* Non-compliant */
  uint32_t *id = (uint32_t *)pkt;
  ```

- **CODE-GUIDE-TYPE-23**: Assign function pointers with matching types.
  ```c
  /* Compliant */
  typedef void (*fn_t)(void);
  fn_t fn_a, fn_b;
  fn_a = fn_b;
  
  /* Non-compliant */
  typedef uint32_t (*fn_b_t)(uint32_t);
  fn_a = fn_b;
  ```

- **CODE-GUIDE-TYPE-24**: No casting function pointers.
  ```c
  /* Compliant */
  typedef uint32_t (*fn_t)(uint32_t);
  uint32_t module_x_pkt_process(uint32_t id) { return id; }
  fn_t fn = module_x_pkt_process;
  
  /* Non-compliant */
  fn = (fn_t)module_x_pkt_process;
  ```

- **CODE-GUIDE-TYPE-25**: Treat string literals as `const`.
  ```c
  /* Compliant */
  const char *str = "test";
  
  /* Non-compliant */
  char *str = "test";
  ```

- **CODE-GUIDE-TYPE-26**: Use `typedef` for numerical types.
  ```c
  /* Compliant */
  typedef uint32_t pkt_id_t;
  pkt_id_t id = 0U;
  
  /* Non-compliant */
  unsigned int id = 0U;
  ```

- **CODE-GUIDE-TYPE-27**: Same types for assignment operands.
  ```c
  /* Compliant */
  uint32_t id = 0U;
  
  /* Non-compliant */
  uint32_t id = 0UL;
  ```

- **CODE-GUIDE-TYPE-28**: Same types for arithmetic operands.
  ```c
  /* Compliant */
  uint32_t a = 1U;
  uint32_t b = (uint32_t)2UL;
  uint32_t sum = a + b;
  
  /* Non-compliant */
  uint32_t sum = a + 2UL;
  ```

- **CODE-GUIDE-TYPE-29**: Use `U`/`UL` suffixes for unsigned constants.
  ```c
  /* Compliant */
  uint32_t value = 10U;
  uint64_t big_value = 10UL;
  
  /* Non-compliant */
  uint32_t value = 10;
  ```

### Identifiers

- **CODE-GUIDE-IDENTIFIER-01**: Unique parameter names.
  ```c
  /* Compliant */
  struct module_x_pkt {
      uint32_t id;
  };
  void module_x_pkt_process(uint32_t count) {}
  
  /* Non-compliant */
  void module_x_pkt_process(uint32_t module_x_pkt) {}
  ```

- **CODE-GUIDE-IDENTIFIER-02**: Unique struct member names.
  ```c
  /* Compliant */
  struct module_x_pkt {
      uint32_t pkt_id;
  };
  
  /* Non-compliant */
  struct module_x_pkt {
      uint32_t module_x_pkt;
  };
  ```

- **CODE-GUIDE-IDENTIFIER-03**: Unique global variable names.
  ```c
  /* Compliant */
  struct module_x_pkt {};
  uint32_t g_module_x_counter;
  
  /* Non-compliant */
  uint32_t g_module_x_pkt;
  ```

- **CODE-GUIDE-IDENTIFIER-04**: Local variables differ from globals.
  ```c
  /* Compliant */
  uint32_t g_module_x_counter;
  void fn(void) {
      uint32_t local_counter;
  }
  
  /* Non-compliant */
  void fn(void) {
      uint32_t g_module_x_counter;
  }
  ```

- **CODE-GUIDE-IDENTIFIER-05**: Unique function names.
  ```c
  /* Compliant */
  uint32_t g_module_x_counter;
  void module_x_pkt_process(void) {}
  
  /* Non-compliant */
  void g_module_x_counter(void) {}
  ```

- **CODE-GUIDE-IDENTIFIER-06**: Unique `typedef` names.
  ```c
  /* Compliant */
  typedef uint32_t pkt_id_t;
  
  /* Non-compliant */
  typedef uint32_t pkt_id_t;
  uint32_t pkt_id_t;
  ```

- **CODE-GUIDE-IDENTIFIER-07**: No leading underscores.
  ```c
  /* Compliant */
  uint32_t pkt_count;
  
  /* Non-compliant */
  uint32_t __pkt_count;
  ```

- **CODE-GUIDE-IDENTIFIER-08**: Variable names differ from structs.
  ```c
  /* Compliant */
  struct module_x_pkt {};
  uint32_t pkt_count;
  
  /* Non-compliant */
  uint32_t module_x_pkt;
  ```

- **CODE-GUIDE-IDENTIFIER-09**: `typedef` names indicate bit size.
  ```c
  /* Compliant */
  typedef uint16_t pkt_id_t;
  
  /* Non-compliant */
  typedef uint16_t pkt_id;
  ```

- **CODE-GUIDE-IDENTIFIER-10**: No redefining C keywords with macros.
  ```c
  /* Compliant */
  typedef _Bool bool_t;
  
  /* Non-compliant */
  #define _Bool bool
  ```

### Coding Style

- **CODE-GUIDE-STYLE-01**: Max 120 characters per line.
  ```c
  /* Compliant */
  uint32_t result = module_x_pkt_process_packet_with_long_name(pkt, ctx, vrf_id);
  
  /* Non-compliant */
  uint32_t result = module_x_pkt_process_packet_with_very_long_name_and_parameters(pkt, ctx, vrf_id, type, size);
  ```

- **CODE-GUIDE-STYLE-02**: One statement per line.
  ```c
  /* Compliant */
  a = 0U;
  b = 0U;
  
  /* Non-compliant */
  a = b = 0U;
  ```

- **CODE-GUIDE-STYLE-03**: Use 4 spaces for indentation.
  ```c
  /* Compliant */
  if (condition) {
      do_something();
  }
  
  /* Non-compliant */
  if (condition) {
    do_something();
  }
  ```

- **CODE-GUIDE-STYLE-04**: Tabs are 4 characters wide (enforced by AStyle).
- **CODE-GUIDE-STYLE-05**: No trailing whitespace.
  ```c
  /* Compliant */
  uint32_t a = 0U;
  
  /* Non-compliant */
  uint32_t a = 0U; /* Trailing space */
  ```

- **CODE-GUIDE-STYLE-06**: Space between keywords and brackets.
  ```c
  /* Compliant */
  if (condition) {
      /* Code */
  }
  
  /* Non-compliant */
  if(condition) {
      /* Code */
  }
  ```

- **CODE-GUIDE-STYLE-07**: No space between function name and parenthesis.
  ```c
  /* Compliant */
  module_x_pkt_process(pkt);
  
  /* Non-compliant */
  module_x_pkt_process (pkt);
  ```

- **CODE-GUIDE-STYLE-08**: No space inside brackets.
  ```c
  /* Compliant */
  module_x_pkt_process(pkt);
  
  /* Non-compliant */
  module_x_pkt_process( pkt );
  ```

- **CODE-GUIDE-STYLE-09**: Place `*` before identifiers.
  ```c
  /* Compliant */
  uint32_t *pkt_ptr;
  
  /* Non-compliant */
  uint32_t* pkt_ptr;
  ```

- **CODE-GUIDE-STYLE-10**: Spaces around binary/ternary operators.
  ```c
  /* Compliant */
  uint32_t result = a + b;
  
  /* Non-compliant */
  uint32_t result = a+b;
  ```

- **CODE-GUIDE-STYLE-11**: No space after unary operators.
  ```c
  /* Compliant */
  uint32_t *ptr;
  uint32_t value = *ptr;
  
  /* Non-compliant */
  uint32_t value = * ptr;
  ```

- **CODE-GUIDE-STYLE-12**: Space after semicolons in `for` loops.
  ```c
  /* Compliant */
  for (uint32_t i = 0U; i < 5U; i++) {
      /* Loop */
  }
  
  /* Non-compliant */
  for (uint32_t i = 0U;i < 5U;i++) {
      /* Loop */
  }
  ```

- **CODE-GUIDE-STYLE-13**: Braces on same line as keywords.
  ```c
  /* Compliant */
  if (condition) {
      /* Code */
  }
  
  /* Non-compliant */
  if (condition)
  {
      /* Code */
  }
  ```

- **CODE-GUIDE-STYLE-14**: Function body starts with opening brace.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_process(void) {
      return 0U;
  }
  
  /* Non-compliant */
  uint32_t module_x_pkt_process(void) { return 0U; }
  ```

- **CODE-GUIDE-STYLE-15**: Align `switch` and `case` statements.
  ```c
  /* Compliant */
  switch (type) {
  case 1:
      break;
  default:
      break;
  }
  
  /* Non-compliant */
  switch (type) {
      case 1:
          break;
      default:
          break;
  }
  ```

- **CODE-GUIDE-STYLE-16**: Align function parameters.
  ```c
  /* Compliant */
  module_x_pkt_process(pkt,
                   ctx,
                   vrf_id);
  
  /* Non-compliant */
  module_x_pkt_process(pkt,
                       ctx,
                     vrf_id);
  ```

- **CODE-GUIDE-STYLE-17**: Use `/* */` for single-line comments.
  ```c
  /* Compliant */
  /* Process packet */
  
  /* Non-compliant */
  // Process packet
  ```

- **CODE-GUIDE-STYLE-18**: Use Doxygen-style comments for functions.
  ```c
  /* Compliant */
  /**
   * @brief Process a packet.
   * @param pkt Packet object.
   * @return 0 on success, -EINVAL on error.
   */
  int32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0;
  }
  
  /* Non-compliant */
  /* Process packet */
  int32_t module_x_pkt_process(struct pkt_obj *pkt) {
      return 0;
  }
  ```

- **CODE-GUIDE-STYLE-19**: Include legal entity in every file.
  ```c
  /* Compliant */
  /*******************************************************************************
   * Copyright© 2025 by Example, Inc. All rights reserved.
   * Licensed under MIT License.
   *******************************************************************************/
  #include <stdint.h>
  
  /* Non-compliant */
  #include <stdint.h> /* No legal entity */
  ```

### Naming Convention

- **CODE-GUIDE-NAMING-01**: Uppercase object-like macros.
  ```c
  /* Compliant */
  #define MAX_PKT_SIZE 1500U
  
  /* Non-compliant */
  #define max_pkt_size 1500U
  ```

- **CODE-GUIDE-NAMING-02**: No mixed case in function-like macros.
  ```c
  /* Compliant */
  #define MAX(a, b) ((a) > (b) ? (a) : (b))
  
  /* Non-compliant */
  #define Max(a, b) ((a) > (b) ? (a) : (b))
  ```

- **CODE-GUIDE-NAMING-03**: Prefix external data structures with `acrn_`.
  ```c
  /* Compliant */
  struct acrn_pkt {
      uint32_t id;
  };
  
  /* Non-compliant */
  struct pkt {
      uint32_t id;
  };
  ```

- **CODE-GUIDE-NAMING-04**: Prefix hypervisor-only structures with `hv_`.
  ```c
  /* Compliant */
  struct hv_timer {
      uint32_t ticks;
  };
  
  /* Non-compliant */
  struct timer {
      uint32_t ticks;
  };
  ```

- **CODE-GUIDE-NAMING-05**: Prefix module-specific structures with module name.
  ```c
  /* Compliant */
  struct module_x_pkt_context {
      uint32_t id;
  };
  
  /* Non-compliant */
  struct context {
      uint32_t id;
  };
  ```

- **CODE-GUIDE-NAMING-06**: Suffix hardware-related structures.
  ```c
  /* Compliant */
  struct lapic_reg {
      uint32_t value;
  };
  
  /* Non-compliant */
  struct lapic {
      uint32_t value;
  };
  ```

- **CODE-GUIDE-NAMING-07**: Suffix function pointers with `fn`.
  ```c
  /* Compliant */
  typedef void (*process_fn)(void);
  
  /* Non-compliant */
  typedef void (*process)(void);
  ```

- **CODE-GUIDE-NAMING-08**: Descriptive function names.
  ```c
  /* Compliant */
  uint32_t module_x_pkt_init_context(uint32_t id);
  
  /* Non-compliant */
  uint32_t init(uint32_t id);
  ```

- **CODE-GUIDE-NAMING-09**: Use prefixes/suffixes for variables.
  ```c
  /* Compliant */
  uint32_t g_module_x_counter;
  struct module_x_pkt st_pkt;
  typedef struct module_x_pkt module_x_pkt_t;
  module_x_pkt_t *p_pkt;
  enum e_module_x_status status;
  
  /* Non-compliant */
  uint32_t counter;
  struct module_x_pkt pkt;
  ```

### Language Extensions

- **CODE-GUIDE-LE-01**: Inline assembly is allowed.
  ```c
  void asm_pause(void) {
      asm volatile ("pause");
  }
  ```

- **CODE-GUIDE-LE-02**: Use `__builtin_va_list`.
  ```c
  void module_x_log_variadic(const char *fmt, ...) {
      __builtin_va_list args;
      __builtin_va_start(args, fmt);
      /* Process */
      __builtin_va_end(args);
  }
  ```

- **CODE-GUIDE-LE-03**: Use extended type attributes (e.g., `aligned`, `packed`).
  ```c
  struct module_x_pkt {
      uint32_t id;
  } __attribute__((packed));
  ```

- **CODE-GUIDE-LE-04**: Use extended built-in functions.
  ```c
  size_t offset = __builtin_offsetof(struct module_x_pkt, id);
  ```

- **CODE-GUIDE-LE-05**: Use designated initializers.
  ```c
  uint32_t buffer[10] = {[0 ... 9] = 0U};
  ```

## Benefits and Considerations

These guidelines ensure:
- **Readability**: Clear naming, consistent formatting, and Doxygen comments make code easy to understand.
- **Debuggability**: Error handling, logging, and single-task functions simplify tracing issues.
- **Maintainability**: Modular code, no duplication, and low complexity reduce maintenance effort.
- **Safety**: Strict type usage, bounds checking, and memory management prevent common C pitfalls.
- **Portability**: Standard types and explicit conversions ensure compatibility across platforms.

Adhering to these rules requires discipline but results in robust, scalable C code suitable for critical systems like modx. Use tools like AStyle, static analyzers (e.g., Clang-Tidy), and code reviews to enforce compliance.