All vector‐file operations live in `VectorIO.hpp`. Two inline functions, `read_vector` and `write_vector`, dispatch on the file extension (via `get_extension()`) to the correct I/O path.


### `write_vector`

```cpp
inline bool write_vector(
    const std::string &filename,
    const std::vector<double> &vec
);
```

- Determine the extension of the file and its size

    ```cpp
    size_t N = vec.size();
    std::string ext = get_extension(filename);
    ```

- If the file is a `.txt` file

    ```cpp
    std::ofstream out_file(filename);
    for (size_t i = 0; i < N; ++i) {
        out_file << vec[i] << "\n";
    }
    out_file.close();
    return true;
    ```

- If the file is a `.dat` file:

    ```cpp
    std::ofstream out_file(filename, std::ios::binary);
    out_file.write(
        reinterpret_cast<const char*>(vec.data()),
        N * sizeof(double)
    );
    out_file.close();
    return true;
    ``` 

- If the file is an HDF5 file:

    ```cpp
    H5::H5File file(filename, H5F_ACC_TRUNC);
    hsize_t dims[1] = { N };
    H5::DataSpace dataspace(1, dims);
    H5::DataSet dataset = file.createDataSet(
        "d", H5::PredType::NATIVE_DOUBLE, dataspace
    );
    dataset.write(vec.data(), H5::PredType::NATIVE_DOUBLE);
    file.close();
    return true;
    ``` 

Any unsupported extension triggers an error message and a `false` return.


### `read_vector`

```cpp
inline bool read_vector(
    const std::string &filename,
    size_t N,
    std::vector<double> &vec
);
```

- Determine the extension of the file and its size

    ```cpp
    std::string ext = get_extension(filename);
    ```

- If the file is a `.txt` file

    ```cpp
    std::ifstream in_file(filename);
    vec.resize(N);
    for (size_t i = 0; i < N; ++i) {
        in_file >> vec[i];
        if (in_file.fail()) return false;
    }
    in_file.close();
    return true;
    ```

- If the file is a `.dat` file:

    ```cpp
    std::ifstream in_file(filename, std::ios::binary);
    vec.resize(N);
    in_file.read(reinterpret_cast<char*>(vec.data()), N * sizeof(double));
    if (in_file.gcount() != static_cast<std::streamsize>(N * sizeof(double)))
        return false;
    in_file.close();
    return true;
    ```

- If the file is an HDF5 file:

    ```cpp
    H5::H5File file(filename, H5F_ACC_RDONLY);
    H5::DataSet dataset = file.openDataSet(ds_name);
    dataset.read(vec.data(), H5::PredType::NATIVE_DOUBLE);
    file.close();
    return true;
    ```

On any unsupported extension or I/O error, an error is logged to `std::cerr` and the function returns `false`.

---

These helpers are invoked by `generateVectors.cpp` when producing the input files and by `vectorSum.cpp` when reading and writing vectors for the sum operation.