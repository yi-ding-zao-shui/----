这个文法描述了一个简单的编程语言的语法结构，我们可以使用自顶向下的递归下降分析法来实现语法分析。具体实现过程如下：

1. 首先需要定义语法树节点的数据结构，以便记录语法树的结构和信息。例如：

```
复制struct TreeNode {
    string name; // 节点名称，表示语法规则的左部
    vector<TreeNode*> children; // 子节点列表
    string value; // 节点值，表示语法规则右部的终结符或者非终结符
};
```

2. 然后需要实现每个语法规则的解析函数，以便将输入的代码串转化为语法树。例如：

```
复制// program -> declarations stmt-sequence
TreeNode* parse_program() {
    TreeNode* node = new TreeNode({"program", {}, ""});
    node->children.push_back(parse_declarations());
    node->children.push_back(parse_stmt_sequence());
    return node;
}

// declarations -> decl ; declarations | ε
TreeNode* parse_declarations() {
    TreeNode* node = new TreeNode({"declarations", {}, ""});
    if (is_type_specifier(peek())) {
        node->children.push_back(parse_decl());
        match(';');
        node->children.push_back(parse_declarations());
    }
    return node;
}

// decl -> type-specifier varlist
TreeNode* parse_decl() {
    TreeNode* node = new TreeNode({"decl", {}, ""});
    node->children.push_back(parse_type_specifier());
    node->children.push_back(parse_varlist());
    return node;
}

// type-specifier -> int | bool | string | float | double
TreeNode* parse_type_specifier() {
    string token = peek();
    if (token == "int" || token == "bool" || token == "string" || token == "float" || token == "double") {
        match(token);
        return new TreeNode({"type-specifier", {}, token});
    }
    else {
        error();
        return nullptr;
    }
}

// varlist -> identifier { , identifier }
TreeNode* parse_varlist() {
    TreeNode* node = new TreeNode({"varlist", {}, ""});
    string token = match("identifier");
    node->children.push_back(new TreeNode({"identifier", {}, token}));
    while (peek() == ",") {
        match(",");
        token = match("identifier");
        node->children.push_back(new TreeNode({"identifier", {}, token}));
    }
    return node;
}

// stmt-sequence -> statement { ; statement }
TreeNode* parse_stmt_sequence() {
    TreeNode* node = new TreeNode({"stmt-sequence", {}, ""});
    node->children.push_back(parse_statement());
    while (peek() == ";") {
        match(";");
        node->children.push_back(parse_statement());
    }
    return node;
}

// statement -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt | while-stmt
TreeNode* parse_statement() {
    string token = peek();
    if (token == "if") {
        return parse_if_stmt();
    }
    else if (token == "repeat") {
        return parse_repeat_stmt();
    }
    else if (is_identifier(token)) {
        return parse_assign_stmt();
    }
    else if (token == "read") {
        return parse_read_stmt();
    }
    else if (token == "write") {
        return parse_write_stmt();
    }
    else if (token == "while") {
        return parse_while_stmt();
    }
    else {
        error();
        return nullptr;
    }
}
```

3. 在解析函数中需要使用一些辅助函数来判断当前输入是否符合某个语法规则，以便进行正确的解析。例如：

```
复制// 判断当前token是否是指定的终结符
bool is_token(string token, string expected) {
    return token == expected;
}

// 判断当前token是否是指定的标识符
bool is_identifier(string token) {
    return isalpha(token[0]);
}

// 判断当前token是否是指定的类型标识符
bool is_type_specifier(string token) {
    return token == "int" || token == "bool" || token == "string" || token == "float" || token == "double";
}

// 判断当前token是否是指定的加法操作符
bool is_addop(string token) {
    return token == "+" || token == "-";
}

// 判断当前token是否是指定的乘法操作符
bool is_mulop(string token) {
    return token == "*" || token == "/";
}
```

4. 最后，我们可以编写一个主函数来调用语法分析器，解析输入的代码串，并输出语法树。例如：

```
复制int main() {
    string code = "int x, y; x := 1; y := 2; while (x < y) { x := x + 1; }";
    init(code);
    TreeNode* root = parse_program();
    print_tree(root, 0);
    return 0;
}
```

在实现了以上这些步骤之后，我们就可以使用C++来实现该语法分析器，对输入的代码进行语法分析，并输出语法树了。
