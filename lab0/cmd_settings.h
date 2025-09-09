namespace lab0
{
    class CmdSettings
    {

    public:
        CmdSettings(int argc, char *argv[]);
        char *get_file_path()
        {
            return filePath;
        }
        char get_file_delim()
        {
            return delim;
        }

    private:
        char *filePath;
        char delim;
    };

}