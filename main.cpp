#include <crow.h>
#include "Headers/LogicCalculator.h"
#include <iostream>

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/evaluate")([](const crow::request &req, crow::response &res){
        std::string expr = req.url_params.get("expr");
        std::cout << "QUERY = " << expr << std::endl;
        res.set_header("Content-Type", "application/json");
        res.write(LogicCalculator::calculate_expression(expr));
        res.end();
    });

    CROW_ROUTE(app, "/")
            ([](){
                return "<div><h1>Use /evaluate directive and query string with parameter expr, which contains expression.</h1><h1>You will get json object with status(OK or error text), matrix of answers and answer string</h1><div>";
            });

    char* port = getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL ? std::stoi(port) : 18080);
    std::cout << "PORT = " << iPort << std::endl;
    app.port(iPort).multithreaded().run();
}