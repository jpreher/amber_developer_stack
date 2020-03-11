/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#ifdef EIGEN_UTILITIES_NDEBUG
    #warning "EIGEN_UTILITIES_NDEBUG was enabled. Using alternative unittests."
#endif
#include <eigen_utilities/runtime_config.hpp>

#include <Eigen/Dense>

#ifndef EIGEN_UTILITIES_NDEBUG

TEST(eigen_utilities_debug, check_flags)
{
    EXPECT_TRUE(eigen_utilities::is_debug_enabled());
}

TEST(eigen_utilities_debug, check_disabled)
{
    EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
    
    {
        eigen_utilities::MallocAllowedScope scope(false);
        EXPECT_TRUE(scope.wasMallocPreviouslyEnabled());
        EXPECT_FALSE(Eigen::internal::is_malloc_allowed());
        
        {
            eigen_utilities::MallocAllowedScope scope(false);
            EXPECT_FALSE(scope.wasMallocPreviouslyEnabled());
            EXPECT_FALSE(Eigen::internal::is_malloc_allowed());
            
            {
                eigen_utilities::MallocAllowedScope scope(true);
                EXPECT_FALSE(scope.wasMallocPreviouslyEnabled());
                EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
            }
        }
        
        EXPECT_FALSE(Eigen::internal::is_malloc_allowed());
    }
    
    EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
}

TEST(eigen_utilities_debug, check_alloc)
{
    /// @note When the exception is thrown during a resize, it dies on the deconstructor
    /// if the variable is scoped outside of the resize() statement. Most likely due to the
    /// catch() for gtest EXPECT_THROW()
    
    // Expect no error
    {
        EXPECT_NO_THROW(
            Eigen::MatrixXd blank;
            blank.resize(2, 2)
        );
    }
    
    // Now expect it to error out
    {
        eigen_utilities::MallocAllowedScope scope(false);
        EXPECT_THROW(
            Eigen::MatrixXd blank;
            blank.resize(4, 4);
        , eigen_utilities::assert_error);
        
        // Check nesting once more
        {
            eigen_utilities::MallocAllowedScope scope(false);
            EXPECT_THROW(
                Eigen::VectorXd vec(5);
            , eigen_utilities::assert_error);
        }
        
        // Check reenabling
        {
            eigen_utilities::MallocAllowedScope scope(true);
            EXPECT_NO_THROW(
                Eigen::VectorXd vec(5);
            );
        }
    }
    
    // Check once again, ensuring that it is still disabled
    {
        EXPECT_NO_THROW(
            Eigen::MatrixXd blank;
            blank.resize(10, 10)
        );
    }
}

/// @warning When using different compiler optimization flags (namely debug), inlined functions may be actually not be inlined
/// For this reason, these 'extern' checks are disabled

#ifdef EIGEN_UTILITIES_TEST_EXTERN
// Forward declare stuff from other source files with different preprocessor values

namespace test_runtime_config_ndebug
{
    /**
     * @brief check_alloc Allocate a dynamically-sized matrix in a source file NDEBUG defined (where malloc() switches should have no effect)
     * @return Return if malloc was previously disabled
     */
    bool resize_matrix_calling_disable();
}

namespace test_runtime_config_clean_eigen
{
    /**
     * @brief check_alloc Allocate a dynamically-sized matrix in a source file with a clean include of Eigen
     */
    void resize_matrix();
}

int resize_matrix()
{
    eigen_utilities::MallocAllowedScope scope(false);
    Eigen::MatrixXd blank;
    blank.resize(5, 5);
}

TEST(eigen_utilities_debug, check_alloc_extern)
{
    // Check alloc when including from source files without the MALLOC definition defined
    {
        eigen_utilities::MallocAllowedScope scope(false);
        
        // Check for source file with NDEBUG defined
        int old_value = true;
        EXPECT_NO_THROW(
            old_value = test_runtime_config_ndebug::resize_matrix_calling_disable();
        );
        // Malloc should not have been disabled by this source file
        EXPECT_TRUE(old_value);
        
        // Check for source file with only Eigen included
        EXPECT_NO_THROW(
            test_runtime_config_clean_eigen::resize_matrix();
        );
        
        // Double-check for functions included in this file
        EXPECT_THROW(
            resize_matrix();
        , eigen_utilities::assert_error);
    }
}
#endif

TEST(eigen_utilities, check_zero)
{
    Eigen::MatrixXd blank(3, 3);
    
    // Ensure that all are initialized to zero
    EXPECT_TRUE((blank.array() == 0).all());
    
    blank.resize(5, 5);
    EXPECT_TRUE((blank.array() == 0).all());
}

//TEST(eigen_utilities, check_nan)
//{
//    Eigen::MatrixXd blank(3, 3);
    
//    // Ensure that all are initialized to nan
//    EXPECT_TRUE((blank.array() != blank.array()).all());
//    std::cout << blank << std::endl;
    
//    blank.resize(5, 5);
//    EXPECT_TRUE((blank.array() != blank.array()).all());
//    std::cout << blank << std::endl;
//}

#else

TEST(eigen_utilities_no_debug, check_flags)
{
    std::cout << "[ warning ] These unittests are for debugging being disabled" << std::endl;
    EXPECT_FALSE(eigen_utilities::is_debug_enabled());
}

TEST(eigen_utilities_no_debug, check_disabled)
{
    EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
    
    {
        eigen_utilities::MallocAllowedScope scope(false);
        EXPECT_TRUE(scope.wasMallocPreviouslyEnabled());
        EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
        
        {
            eigen_utilities::MallocAllowedScope scope(false);
            EXPECT_TRUE(scope.wasMallocPreviouslyEnabled());
            EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
        }
        
        EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
    }
    
    EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
}

TEST(eigen_utilities_no_debug, check_alloc)
{
    // Expect no errors at all
    
    {
        EXPECT_NO_THROW(
            Eigen::MatrixXd blank;
            blank.resize(2, 2)
        );
    }
    
    {
        eigen_utilities::MallocAllowedScope scope(false);
        EXPECT_NO_THROW(
            Eigen::MatrixXd blank;
            blank.resize(4, 4);
        );
        
        // Check nesting once more
        {
            eigen_utilities::MallocAllowedScope scope(false);
            EXPECT_NO_THROW(
                Eigen::VectorXd vec(5);
            );
        }
    }
    
    {
        EXPECT_NO_THROW(
            Eigen::MatrixXd blank;
            blank.resize(10, 10)
        );
    }
}

#endif

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
