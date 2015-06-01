#pragma once

namespace tools {

    void execute(const std::string & file, const std::string & arg) {
        auto pid = fork();

        if (pid == -1)
            perror(nullptr);
        else if (pid == 0) {
            const char * const args[3] = {
                file.c_str(),
                arg.c_str(),
                nullptr
            };
            execvp(file.c_str(), const_cast<char **>(args));
            std::cerr << "Error executing " << file << ": ";
            perror(nullptr);
            exit(1);
        }
        else
            waitpid(pid, nullptr, 0);
    }

}
