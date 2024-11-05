
```mermaid
classDiagram
    %% High-level components and relationships in main.cpp

    class Config {
        %% Configuration for data requests
        + cache: bool
        + start_date: std::string
        + end_date: std::string
    }

    class DataFrame {
        %% Represents tabular data with columns as Series
        + to_excel(filename: std::string): void
        + print(): void
        + show(): void
        + operator[](col: std::string): Series
    }

    class Series {
        %% Represents a single column of data in DataFrame
        + values(): std::vector<double>
        + size(): size_t
        + at(index: size_t): T
        + print(): void
    }

    class get_series {
        %% Fetches data based on the index and configuration
        + get_series(index: std::string, config: Config): DataFrame
    }

    class getShortFilename {
        %% Utility function to generate concise filenames
        + getShortFilename(input: std::string): std::string
    }

    class check_df {
        %% Utility function to check a specific column in DataFrame
        + check_df(df: DataFrame, col: std::string): std::vector<double>
    }

    %% External Dependencies
    class JSONParser {
        %% External library for JSON parsing
        + parse(res: std::string): JSONParser
    }

    %% Define Relationships Between Components
    main --> Config : creates
    main --> get_series : calls
    get_series --> DataFrame : returns
    DataFrame --> Series : holds columns as Series
    main --> check_df : calls
    main --> getShortFilename : calls
    check_df --> Series : retrieves column values
    get_series --> JSONParser : parses response
