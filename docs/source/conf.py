# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information


project = 'Custom IC Creator'
copyright = '2022, Carsten Wulff'
author = 'Carsten Wulff'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [ "breathe"]


breathe_projects = {"ciccreator": "../../doxygen/xml/"}

breathe_default_project = "ciccreator"

breathe_implementation_filename_extensions = ['.c', '.cc', '.cpp']

templates_path = ['_templates']
exclude_patterns = []

html_sidebars = {
    '**': [
        'globaltoc.html',
    ]
}

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'nature'
html_static_path = ['_static']
